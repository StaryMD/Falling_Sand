#include "FallingSandEngine.hpp"

#include <CL/cl.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <CL/opencl.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "CommonUtility.hpp"
#include "MasterEngine/CameraView.hpp"
#include "MasterEngine/GameEngine.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"

FallingSandEngine::FallingSandEngine(const std::string& application_name, const sf::Vector2<uint32_t> world_size)
    : GameEngine(application_name), world_(world_size), screen_size_(window_.getSize()) {
  Setup();
}

void FallingSandEngine::Setup() {
  try {
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
        std::cerr << "No devices have been found\n";
      }

      d_context_ = cl::Context(best_device);
    }

    d_queue_ = cl::CommandQueue(d_context_);
    d_input_buffer_ = cl::Buffer(d_context_, CL_MEM_READ_ONLY, world_.GetElementCount() * sizeof(Element));
    d_output_buffer_ = cl::Buffer(d_context_, CL_MEM_WRITE_ONLY, GetPixelCount() * sizeof(sf::Color));

    const std::string kernel_source =
        ReadFileContent(std::filesystem::current_path().string() + "/assets/kernels/PaintOn.cl");
    const cl::Program program(d_context_, kernel_source);

    std::stringstream build_options;
    build_options << "-DSCREEN_SIZE_X=" << screen_size_.x << " -DSCREEN_SIZE_Y=" << screen_size_.y
                  << " -DWORLD_SIZE_X=" << world_.GetSize().x << " -DWORLD_SIZE_Y=" << world_.GetSize().y;

    program.build(build_options.str().c_str());

    d_kernel_ = cl::Kernel(program, "PaintOn");

  } catch (const cl::BuildError& error) {
    const cl::BuildLogType build_logs = error.getBuildLog();

    for (const auto& [device, log] : build_logs) {
      std::cerr << "Device " << device.getInfo<CL_DEVICE_NAME>() << " logs:\n" << log << '\n';
    }
    throw;
  }

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

  window_.setMouseCursorVisible(false);
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

bool FallingSandEngine::IsChunkActive(const sf::Vector2<int32_t> position) const {
  return world_.IsChunkActive(position);
}
