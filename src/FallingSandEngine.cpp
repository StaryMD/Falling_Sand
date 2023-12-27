#include "FallingSandEngine.hpp"
#include <CL/cl.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <CL/cl2.hpp>

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "CommonUtility.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"

FallingSandEngine::FallingSandEngine(const sf::Vector2i size, const sf::Vector2u screen_size)
    : world_(size), screen_size_(screen_size) {
  Setup();
}

FallingSandEngine::FallingSandEngine(const sf::Vector2u size, const sf::Vector2u screen_size)
    : world_(size), screen_size_(screen_size) {
  Setup();
}

void FallingSandEngine::Setup() {
  {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    cl::Device best_device;
    size_t device_speed_best = 0;

    for (const cl::Platform& platform : platforms) {
      std::vector<cl::Device> devices;
      platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

      for (const cl::Device& device : devices) {
        const size_t max_compute_units = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
        const size_t max_clock_frequency = device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();

        const size_t device_speed = max_compute_units * max_clock_frequency;

        if (device_speed > device_speed_best) {
          device_speed_best = device_speed;
          best_device = device;
        }
      }
    }

    if (best_device.get() == nullptr) {
      std::cout << "No devices have been found\n";
    }

    d_context_ = cl::Context(best_device);

    std::cout << "Using device: " << best_device.getInfo<CL_DEVICE_NAME>() << '\n';
  }

  d_queue_ = cl::CommandQueue(d_context_);
  d_input_buffer_ = cl::Buffer(d_context_, CL_MEM_READ_ONLY, world_.GetElementCount() * sizeof(Element));
  d_output_buffer_ = cl::Buffer(d_context_, CL_MEM_WRITE_ONLY, GetPixelCount() * sizeof(sf::Color));

  const std::string kernel_source =
      ReadFileContent(std::filesystem::current_path().string() + "/" + "assets/kernels/PaintOn.cl");
  const cl::Program program(d_context_, kernel_source);

  std::stringstream build_options;
  build_options << "-DSCREEN_SIZE_X=" << screen_size_.x << " -DSCREEN_SIZE_Y=" << screen_size_.y
                << " -DWORLD_SIZE_X=" << world_.GetSize().x << " -DWORLD_SIZE_Y=" << world_.GetSize().y;

  try {
    program.build(build_options.str().c_str());
  } catch (const cl::BuildError& error) {
    const auto build_logs = error.getBuildLog();

    for (const auto& [device, log] : build_logs) {
      std::cout << "Device " << device.getInfo<CL_DEVICE_NAME>() << " logs:\n" << log << '\n';
    }
    throw;
  }

  d_kernel_ = cl::Kernel(program, "PaintOn");
}

void FallingSandEngine::PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes,
                                const sf::Vector2u screen_size, const unsigned tick_counter) {
  const sf::Rect<double> view = camera_view.GetFieldOfView();
  const double step_x = view.width / screen_size.x;
  const double step_y = view.height / screen_size.y;

#if USE_OPENCL_FOR_DRAW

  d_queue_.enqueueWriteBuffer(d_input_buffer_, CL_FALSE, 0, world_.GetElementCount() * sizeof(Element),
                              world_.GetElementsPointer());

  int arg_counter = 0;
  try {
    d_kernel_.setArg(arg_counter++, d_input_buffer_);
    d_kernel_.setArg(arg_counter++, d_output_buffer_);
    d_kernel_.setArg(arg_counter++, static_cast<float>(view.left));
    d_kernel_.setArg(arg_counter++, static_cast<float>(view.top));
    d_kernel_.setArg(arg_counter++, static_cast<float>(step_x));
    d_kernel_.setArg(arg_counter++, static_cast<float>(step_y));
    d_kernel_.setArg(arg_counter++, tick_counter);

  } catch (const cl::Error& error) {
    std::cout << "Opencl Error : " << error.err() << " : " << error.what() << '\n';
    std::cout << "Arg counter : " << arg_counter - 1 << '\n';
    throw;
  }

  d_queue_.enqueueNDRangeKernel(d_kernel_, cl::NullRange, cl::NDRange(screen_size.x, screen_size.y));

  d_queue_.enqueueReadBuffer(d_output_buffer_, CL_FALSE, 0, GetPixelCount() * sizeof(sf::Color), bytes.data());

  d_queue_.finish();

#else

  // FORMAT : 0x<A><B><G><R>U
  static const std::array<sf::Color, 9> kColors = {
      sf::Color(0x000000FFU), sf::Color(0xD0D0D0FFU), sf::Color(0x000000FFU),
      sf::Color(0x000000FFU), sf::Color(0x000000FFU), sf::Color(0x000000FFU),
      sf::Color(0x000000FFU), sf::Color(0x000000FFU), sf::Color(0x3B3B38FFU)};

  static const std::array<sf::Color, 4> kSandColors = {sf::Color(0xD8A969FFU), sf::Color(0xEABC7AFFU),
                                                       sf::Color(0xF4C790FFU), sf::Color(0xEABC7AFFU)};
  static const std::array<sf::Color, 4> kStoneColors = {sf::Color(0x636365FFU), sf::Color(0x66676CFFU),
                                                        sf::Color(0x545253FFU), sf::Color(0x646466FFU)};
  static const std::array<sf::Color, 4> kWaterColors = {sf::Color(0x042898FFU), sf::Color(0x0531AAFFU),
                                                        sf::Color(0x052A9DFFU), sf::Color(0x032590FFU)};
  static const std::array<sf::Color, 4> kOilColors = {sf::Color(0xCBAA3FFFU), sf::Color(0xC9A73BFFU),
                                                      sf::Color(0xC8A436FFU), sf::Color(0xC19F33FFU)};
  static const std::array<sf::Color, 4> kSteamColors = {sf::Color(0xDCDFE4FFU), sf::Color(0xE1E3E8FFU),
                                                        sf::Color(0xE5E8EDFFU), sf::Color(0xD7DADFFFU)};
  static const std::array<sf::Color, 4> kFireColors = {sf::Color(0xFF7500FFU), sf::Color(0xFC6400FFU),
                                                       sf::Color(0xD73502FFU), sf::Color(0xB62203FFU)};
  static const std::array<sf::Color, 4> kSmokeColors = {sf::Color(0x090909FFU), sf::Color(0x0A0A0AFFU),
                                                        sf::Color(0x0C0C0CFFU), sf::Color(0x0A0A0AFFU)};

  sf::Color* pixel_it = reinterpret_cast<sf::Color*>(bytes.data());

  double world_coord_y = view.top;

  for (unsigned screen_y = 0; screen_y < screen_size.y; ++screen_y, world_coord_y += step_y) {
    double world_coord_x = view.left;

    for (unsigned screen_x = 0; screen_x < screen_size.x; ++screen_x, world_coord_x += step_x) {
      const Element element = world_.GetElementAt({static_cast<int>(world_coord_x), static_cast<int>(world_coord_y)});

      const auto subs = element.GetSubstance();

      const uint8_t draw_property = element.GetDrawProperty();

      switch (element.GetSubstance()) {
        case engine::Substance::kSand: {
          *(pixel_it++) = kSandColors[draw_property % 4];
          break;
        }
        case engine::Substance::kStone: {
          *(pixel_it++) = kStoneColors[draw_property % 4];
          break;
        }
        case engine::Substance::kWater: {
          const unsigned index = ((tick_counter + draw_property) / 20) % 4;
          *(pixel_it++) = kWaterColors[index];
          break;
        }
        case engine::Substance::kOil: {
          const unsigned index = ((tick_counter + draw_property) / 50) % 4;
          *(pixel_it++) = kOilColors[index];
          break;
        }
        case engine::Substance::kSteam: {
          const unsigned index = ((tick_counter + draw_property) / 30) % 4;
          *(pixel_it++) = kSteamColors[index];
          break;
        }
        case engine::Substance::kFire: {
          const unsigned index = ((tick_counter + draw_property) / 40) % 4;
          *(pixel_it++) = kFireColors[index];
          break;
        }
        case engine::Substance::kSmoke: {
          const unsigned index = ((tick_counter + draw_property) / 40) % 4;
          *(pixel_it++) = kSmokeColors[index];
          break;
        }
        default: {
          *(pixel_it++) = kColors[static_cast<int>(subs)];
        }
      }
    }
  }

#endif
}

void FallingSandEngine::PlaceElementInLine(const sf::Vector2i start_pos, const sf::Vector2i end_pos, const int radius,
                                           const engine::Substance substance) {
  ExecuteInALine(start_pos, end_pos, [&](const float point_on_line_x, const float point_on_line_y) {
    ExecuteInADisc(radius, [&](const int point_on_disc_x, const int point_on_disc_y) {
      world_.SetElementAt(ToVector2<int>(sf::Vector2f(point_on_line_x + static_cast<float>(point_on_disc_x),
                                                      point_on_line_y + static_cast<float>(point_on_disc_y))),
                          Element(substance));
    });
  });
}

void FallingSandEngine::Update() {
  world_.Update();
}

bool FallingSandEngine::IsChunkActive(const sf::Vector2i position) const {
  return world_.IsChunkActive(position);
}

template <typename functor>
void ExecuteInALine(const sf::Vector2i start_point, const sf::Vector2i end_point, const functor& do_function) {
  float move_x = static_cast<float>(end_point.x - start_point.x);
  float move_y = static_cast<float>(end_point.y - start_point.y);

  const int step_count = static_cast<int>(std::max(std::abs(move_x), std::abs(move_y)));

  move_x /= static_cast<float>(step_count);
  move_y /= static_cast<float>(step_count);

  float point_on_line_x = static_cast<float>(start_point.x);
  float point_on_line_y = static_cast<float>(start_point.y);

  for (int i = 0; i <= step_count; i++) {
    do_function(point_on_line_x, point_on_line_y);
    point_on_line_x += move_x;
    point_on_line_y += move_y;
  }
}

template <typename functor>
void ExecuteInADisc(const int radius, const functor& do_function) {
  if (radius == 0) {
    do_function(0, 0);
    return;
  }

  //NOLINTBEGIN(readability-identifier-length)
  int t1 = static_cast<int>(std::sqrt(radius));
  int x = radius;
  int y = 0;

  while (x >= y) {
    {
      for (int i = -x; i <= x; ++i) {
        do_function(i, y);

        if (y != 0) {
          do_function(i, -y);
        }
      }
    }

    ++y;
    t1 += y;
    const int t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;

      for (int i = -y + 1; i <= y - 1; ++i) {
        if (x != (y - 1)) {
          do_function(i, x);
          do_function(i, -x);
        }
      }

      --x;
    }
  }
  //NOLINTEND(readability-identifier-length)
}
