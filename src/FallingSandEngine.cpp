#include "FallingSandEngine.hpp"

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <CL/cl.h>
#include <CL/cl_platform.h>
#include <CL/opencl.hpp>

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
  const cl::Program program(kernel_source);

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

  d_queue_.enqueueWriteBuffer(d_input_buffer_, CL_TRUE, 0, world_.GetElementCount() * sizeof(Element),
                              world_.GetElementsPointer());

  cl_uint arg_counter = 0;
  d_kernel_.setArg(arg_counter++, d_input_buffer_);
  d_kernel_.setArg(arg_counter++, d_output_buffer_);
  d_kernel_.setArg(arg_counter++, static_cast<float>(view.left));
  d_kernel_.setArg(arg_counter++, static_cast<float>(view.top));
  d_kernel_.setArg(arg_counter++, static_cast<float>(step_x));
  d_kernel_.setArg(arg_counter++, static_cast<float>(step_y));
  d_kernel_.setArg(arg_counter++, tick_counter);

  d_queue_.enqueueNDRangeKernel(d_kernel_, cl::NullRange, cl::NDRange(screen_size.x, screen_size.y));

  d_queue_.enqueueReadBuffer(d_output_buffer_, CL_TRUE, 0, GetPixelCount() * sizeof(sf::Color), bytes.data());

  d_queue_.finish();
}

void FallingSandEngine::PlaceElementInLine(const sf::Vector2i start_pos, const sf::Vector2i end_pos,
                                           const engine::Substance substance) {
  ExecuteInALine(start_pos, end_pos, [&](const float point_on_line_x, const float point_on_line_y) {
    world_.SetElementAt(ToVector2<int, float>({point_on_line_x, point_on_line_y}), Element(substance));
  });
}

void FallingSandEngine::Update() {
  world_.Update();
}

bool FallingSandEngine::IsChunkActive(const sf::Vector2i position) const {
  return world_.IsChunkActive(position);
}
