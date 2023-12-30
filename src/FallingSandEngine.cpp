#include "FallingSandEngine.hpp"
#include <CL/cl.h>

#include <array>
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

#include "CommonUtility.hpp"
#include "MasterEngine/CameraView.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"

constexpr uint32_t kChunkColorOpacity = 0x00000033;
constexpr uint32_t kChunkActiveColor = 0x00FF0000 | kChunkColorOpacity;
constexpr uint32_t kChunkInactiveColor = 0xFF000000 | kChunkColorOpacity;

constexpr uint32_t kChunkBorderTransparency = 0x00000033;
constexpr uint32_t kChunkBorderColor = 0x00000000 | kChunkBorderTransparency;

FallingSandEngine::FallingSandEngine(const std::string& application_name, const sf::Vector2<uint32_t> world_size)
    : GameEngine(application_name), world_(world_size), screen_size_(window_.getSize()) {
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

  window_.setVerticalSyncEnabled(true);
  screen_texture_.create(window_.getSize().x, window_.getSize().y);
  screen_sprite_.setTexture(screen_texture_);

  const bool font_loaded_successfully =
      font_.loadFromFile(std::filesystem::current_path().string() + "/assets/fonts/consola.ttf");

  if (not font_loaded_successfully) {
    std::cerr << "Font could not be loaded\n";
  }

  screen_pixels_.resize(static_cast<size_t>(window_.getSize().x * window_.getSize().y) * sizeof(sf::Color));

  text_.setFont(font_);
  text_.setCharacterSize(18U);
}

void FallingSandEngine::UserHandleInput() {
  if (this->window_.hasFocus()) {
    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Key::Escape)) {
      window_.close();
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::F2)) {
      do_compute_next_frame_ = !do_compute_next_frame_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::F3)) {
      do_show_debug_screen_ = !do_show_debug_screen_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::F5)) {
      do_show_chunk_borders_ = !do_show_chunk_borders_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::F7)) {
      do_show_chunk_activity_ = !do_show_chunk_activity_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Space)) {
      if (!do_compute_next_frame_) {
        do_advance_one_frame_ = true;
      }
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Up)) {
      ++world_.update_threads_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Down)) {
      world_.update_threads_ = std::max(0, world_.update_threads_ - 1);
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Left)) {
      ++brush_radius_;
    }

    if (this->event_handler_.IsKeyPressed(sf::Keyboard::Right)) {
      brush_radius_ = std::max(0, brush_radius_ - 1);
    }

    if (this->event_handler_.IsKeyDown(sf::Keyboard::Num1)) {
      chosen_substance_ = engine::Substance::kAir;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num2)) {
      chosen_substance_ = engine::Substance::kSand;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num3)) {
      chosen_substance_ = engine::Substance::kStone;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num4)) {
      chosen_substance_ = engine::Substance::kWater;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num5)) {
      chosen_substance_ = engine::Substance::kOil;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num6)) {
      chosen_substance_ = engine::Substance::kSteam;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num7)) {
      chosen_substance_ = engine::Substance::kFire;
    } else if (this->event_handler_.IsKeyDown(sf::Keyboard::Num8)) {
      chosen_substance_ = engine::Substance::kSmoke;
    }

    if (this->event_handler_.IsMouseButtonDown(sf::Mouse::Button::Left)) {
      const auto drag = this->event_handler_.GetMouseMovement();

      PlaceElementInLine(ToVector2<int32_t>(camera_view_.MapPixelToCoords(drag.first)),
                         ToVector2<int32_t>(camera_view_.MapPixelToCoords(drag.second)), brush_radius_,
                         chosen_substance_);
    }

    if (this->event_handler_.IsMouseButtonDown(sf::Mouse::Button::Middle)) {
      camera_view_.MovePosition(this->event_handler_.GetMouseMovementDelta());
    }

    if (this->event_handler_.GetMouseWheelScrollDelta() != 0) {
      camera_view_.Zoom(this->event_handler_.GetMouseWheelScrollDelta(),
                        this->event_handler_.GetMouseScrollWheelLocation());
    }
  }
}

void FallingSandEngine::UserComputeFrame() {
  if (do_compute_next_frame_ || do_advance_one_frame_) {
    do_advance_one_frame_ = false;

    world_.Update();

    ++tick_count_;
  }
}

void FallingSandEngine::UserDrawFrame() {
  PaintOn(camera_view_, screen_pixels_, this->window_.getSize(), tick_count_);
  screen_texture_.update(screen_pixels_.data());
  window_.draw(screen_sprite_);

  ShowChunkActivity();
  ShowChunkBorders();
  ShowDebugInfo();
  DrawBrush();

  window_.display();
}

void FallingSandEngine::PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes,
                                const sf::Vector2<uint32_t> screen_size, const uint32_t tick_counter) {
  const sf::Rect<double> view = camera_view.GetFieldOfView();
  const double step_x = view.width / screen_size.x;
  const double step_y = view.height / screen_size.y;

#if USE_OPENCL_FOR_DRAW

  d_queue_.enqueueWriteBuffer(d_input_buffer_, CL_FALSE, 0, world_.GetElementCount() * sizeof(Element),
                              world_.GetElementsPointer());

  int32_t arg_counter = 0;
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

  for (uint32_t screen_y = 0; screen_y < screen_size.y; ++screen_y, world_coord_y += step_y) {
    double world_coord_x = view.left;

    for (uint32_t screen_x = 0; screen_x < screen_size.x; ++screen_x, world_coord_x += step_x) {
      const Element element =
          world_.GetElementAt({static_cast<int32_t>(world_coord_x), static_cast<int32_t>(world_coord_y)});

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
          const uint32_t index = ((tick_counter + draw_property) / 20) % 4;
          *(pixel_it++) = kWaterColors[index];
          break;
        }
        case engine::Substance::kOil: {
          const uint32_t index = ((tick_counter + draw_property) / 50) % 4;
          *(pixel_it++) = kOilColors[index];
          break;
        }
        case engine::Substance::kSteam: {
          const uint32_t index = ((tick_counter + draw_property) / 30) % 4;
          *(pixel_it++) = kSteamColors[index];
          break;
        }
        case engine::Substance::kFire: {
          const uint32_t index = ((tick_counter + draw_property) / 40) % 4;
          *(pixel_it++) = kFireColors[index];
          break;
        }
        case engine::Substance::kSmoke: {
          const uint32_t index = ((tick_counter + draw_property) / 40) % 4;
          *(pixel_it++) = kSmokeColors[index];
          break;
        }
        default: {
          *(pixel_it++) = kColors[static_cast<int32_t>(subs)];
        }
      }
    }
  }

#endif
}

void FallingSandEngine::PlaceElementInLine(const sf::Vector2<int32_t> start_pos, const sf::Vector2<int32_t> end_pos,
                                           const int32_t radius, const engine::Substance substance) {
  ExecuteInALine(start_pos, end_pos, [&](const float point_on_line_x, const float point_on_line_y) {
    ExecuteInADisc(radius, [&](const int32_t point_on_disc_x, const int32_t point_on_disc_y) {
      world_.SetElementAt(ToVector2<int32_t>(sf::Vector2f(point_on_line_x + static_cast<float>(point_on_disc_x),
                                                          point_on_line_y + static_cast<float>(point_on_disc_y))),
                          Element(substance));
    });
  });
}

bool FallingSandEngine::IsChunkActive(const sf::Vector2<int32_t> position) const {
  return world_.IsChunkActive(position);
}

void FallingSandEngine::ShowChunkActivity() {
  if (do_show_chunk_activity_) {
    const float chunk_border_size = static_cast<float>(constants::kChunkSize * camera_view_.GetZoomLevel());
    const sf::Rect<double> fov = camera_view_.GetFieldOfView();

    const sf::Vector2<double> offset = {fov.left - std::fmod(fov.left, constants::kChunkSize),
                                        fov.top - std::fmod(fov.top, constants::kChunkSize)};

    const sf::Vector2f pixel_offset = ToVector2<float>(camera_view_.MapCoordsToPixel(offset));

    sf::RectangleShape rect_shape({chunk_border_size, chunk_border_size});
    rect_shape.setOrigin(-pixel_offset);

    const sf::Vector2<int32_t> top_left_chunk_pos =
        ToVector2<int, double>({fov.left / constants::kChunkSize, fov.top / constants::kChunkSize});

    const int32_t visible_chunk_count_x =
        static_cast<int32_t>(static_cast<float>(window_.getSize().x) / chunk_border_size) + 2;
    const int32_t visible_chunk_count_y =
        static_cast<int32_t>(static_cast<float>(window_.getSize().y) / chunk_border_size) + 2;

    for (int32_t relative_chunk_y = 0; relative_chunk_y < visible_chunk_count_y; ++relative_chunk_y) {
      int32_t consecutive_start = 0;
      bool consecutive_activeness = IsChunkActive(top_left_chunk_pos + sf::Vector2<int32_t>{0, relative_chunk_y});

      for (int32_t relative_chunk_x = 1; relative_chunk_x < visible_chunk_count_x; ++relative_chunk_x) {
        const sf::Vector2<int32_t> chunk_pos =
            top_left_chunk_pos + sf::Vector2<int32_t>(relative_chunk_x, relative_chunk_y);

        const bool active = IsChunkActive(chunk_pos);

        if (active != consecutive_activeness) {
          const int32_t consecutive_length = relative_chunk_x - consecutive_start;

          rect_shape.setSize(sf::Vector2f(static_cast<float>(consecutive_length), 1) * chunk_border_size);

          const sf::Vector2f screen_chunk_pos =
              ToVector2<float>(sf::Vector2(consecutive_start, relative_chunk_y)) * chunk_border_size;

          rect_shape.setPosition(screen_chunk_pos);

          if (consecutive_activeness) {
            rect_shape.setFillColor(sf::Color(kChunkActiveColor));
          } else {
            rect_shape.setFillColor(sf::Color(kChunkInactiveColor));
          }

          window_.draw(rect_shape);

          consecutive_start = relative_chunk_x;
          consecutive_activeness = active;
        }
      }
      {
        const int32_t consecutive_length = visible_chunk_count_x - consecutive_start;

        rect_shape.setSize(sf::Vector2f(static_cast<float>(consecutive_length), 1) * chunk_border_size);

        const sf::Vector2f screen_chunk_pos =
            ToVector2<float, int>({consecutive_start, relative_chunk_y}) * chunk_border_size;

        rect_shape.setPosition(screen_chunk_pos);

        if (consecutive_activeness) {
          rect_shape.setFillColor(sf::Color(kChunkActiveColor));
        } else {
          rect_shape.setFillColor(sf::Color(kChunkInactiveColor));
        }

        window_.draw(rect_shape);
      }
    }
  }
}

void FallingSandEngine::ShowChunkBorders() {
  if (do_show_chunk_borders_) {
    const float chunk_border_size = static_cast<float>(constants::kChunkSize * camera_view_.GetZoomLevel());
    const sf::Rect<double> fov = camera_view_.GetFieldOfView();

    const sf::Vector2<double> offset = {fov.left - std::fmod(fov.left, constants::kChunkSize),
                                        fov.top - std::fmod(fov.top, constants::kChunkSize)};

    const sf::Vector2f pixel_offset = ToVector2<float>(camera_view_.MapCoordsToPixel(offset));

    const int32_t line_count_x = static_cast<int32_t>(static_cast<float>(window_.getSize().x) / chunk_border_size) + 1;
    const int32_t line_count_y = static_cast<int32_t>(static_cast<float>(window_.getSize().y) / chunk_border_size) + 1;

    std::array<sf::Vertex, 2> line;

    line[0] = sf::Vertex({pixel_offset.x, 0}, sf::Color(kChunkBorderColor));
    line[1] = sf::Vertex({pixel_offset.x, static_cast<float>(window_.getSize().y)}, sf::Color(kChunkBorderColor));

    for (int32_t i = 0; i < line_count_x; ++i) {
      line[0].position.x += chunk_border_size;
      line[1].position.x += chunk_border_size;

      window_.draw(line.data(), 2, sf::Lines);
    }

    line[0] = sf::Vertex({0, pixel_offset.y}, sf::Color(0, 0, 0, kChunkBorderTransparency));
    line[1] = sf::Vertex({static_cast<float>(window_.getSize().x), pixel_offset.y},
                         sf::Color(0, 0, 0, kChunkBorderTransparency));

    for (int32_t i = 0; i < line_count_y; ++i) {
      line[0].position.y += chunk_border_size;
      line[1].position.y += chunk_border_size;

      window_.draw(line.data(), 2, sf::Lines);
    }
  }
}

void FallingSandEngine::ShowDebugInfo() {
  if (do_show_debug_screen_) {
    static std::string text_string;

    ConstructDebugText(text_string);

    text_.setString(text_string);

    text_.setFillColor(sf::Color::Black);
    text_.setPosition(3, 3);
    window_.draw(text_);

    text_.setFillColor(sf::Color::White);
    text_.setPosition(1, 1);
    window_.draw(text_);
  }
}

void FallingSandEngine::DrawBrush() {
  const int32_t pixel_size = static_cast<int32_t>(camera_view_.GetZoomLevel());
  const sf::Vector2<int32_t> mouse_pos = event_handler_.GetMousePosition();

  const sf::Vector2<int32_t> coord = ToVector2<int32_t>(camera_view_.MapPixelToCoords(mouse_pos));
  const sf::Vector2<int32_t> pointer_tile = camera_view_.MapCoordsToPixel(ToVector2<double>(coord));

  static sf::RectangleShape rect_shape;
  rect_shape.setSize(ToVector2<float>(sf::Vector2(pixel_size, pixel_size)));

  const auto current_frame_count = refresh_rate_.GetFrameCount();
  if (current_frame_count % 100 < 50) {
    rect_shape.setFillColor(sf::Color(255, 255, 255, 75 + current_frame_count % 50));
  } else {
    rect_shape.setFillColor(sf::Color(255, 255, 255, 124 - current_frame_count % 50));
  }

  ExecuteInACircle(brush_radius_, [&](const int32_t point_on_circle_x, const int32_t point_on_circle_y) {
    rect_shape.setPosition(
        ToVector2<float>(pointer_tile + sf::Vector2<int32_t>(point_on_circle_x, point_on_circle_y) * pixel_size));
    window_.draw(rect_shape);
  });
}

void FallingSandEngine::ConstructDebugText(std::string& text_string) const {
  const auto [avg_fps, min_fps] = refresh_rate_.GetFpsInfo();
  const double total_frame_elapsed_time = total_frame_elapsed_time_;
  const double total_frame_percentage = total_frame_elapsed_time / constants::kWantedSecondsPerFrame * 100.0;
  const double handle_events_elapsed_percentage = handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_percentage = draw_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double compute_elapsed_percentage = compute_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const uint32_t updated_chunks_count = GetUpdatedChunksCount();

  const sf::Vector2<int32_t> mouse_position =
      ToVector2<int32_t>(window_.mapPixelToCoords(sf::Mouse::getPosition(window_)));
  const sf::Vector2<double> mouse_coord = camera_view_.MapPixelToCoords(mouse_position);
  const sf::Vector2<double> camera_fov = {camera_view_.GetFieldOfView().left, camera_view_.GetFieldOfView().top};

  text_string.resize(0);

  std::ostringstream debug_string(text_string);

  debug_string << std::setprecision(constants::kDebugDigitPrecision) << std::fixed
               << "FC: " << refresh_rate_.GetFrameCount() << '\n'
               << "TC: " << tick_count_ << '\n'
               << '\n'
               << "FPS AVG: " << avg_fps << '\n'
               << "FPS MIN: " << min_fps << '\n'
               << "TOTAL FRAME TIME: " << total_frame_elapsed_time << '\n'
               << "TOTAL FRAME %: " << total_frame_percentage << '\n'
               << '\n'
               << "TOTAL EVENT %: " << handle_events_elapsed_percentage << '\n'
               << "TOTAL COMP %: " << compute_elapsed_percentage << '\n'
               << "TOTAL DRAW %: " << screen_update_elapsed_percentage << '\n'
               << '\n'
               << "MOUSE PIXEL : " << mouse_position.x << ' ' << mouse_position.y << '\n'
               << "CAMERA POS : " << camera_fov.x << ' ' << camera_fov.y << '\n'
               << "MOUSE COORD : " << mouse_coord.x << ' ' << mouse_coord.y << '\n'
               << '\n'
               << "CHUNKS UPDATED : " << updated_chunks_count << '\n'
               << "UPDATE THREADS : " << GetWorld().update_threads_ << '\n'
               << "BRUSH RADIUS : " << brush_radius_ << '\n';

  text_string = debug_string.str();
}
