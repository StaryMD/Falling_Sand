#include "FallingSandEngine.hpp"

#include <CL/cl.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <CL/opencl.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "CommonConstants.hpp"
#include "CommonUtility.hpp"
#include "MasterEngine/CameraView.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"

constexpr uint32_t kChunkColorOpacity = 0x00000033;
constexpr uint32_t kChunkActiveColor = 0x00FF0000 | kChunkColorOpacity;
constexpr uint32_t kChunkInactiveColor = 0xFF000000 | kChunkColorOpacity;

constexpr uint32_t kChunkBorderTransparency = 0x00000033;
constexpr uint32_t kChunkBorderColor = 0x00000000 | kChunkBorderTransparency;

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
                                const sf::Vector2<uint32_t> screen_size,
                                const uint32_t tick_counter) {
  const sf::Rect<double> view = camera_view.GetFieldOfView();
  const double step_x = view.width / screen_size.x;
  const double step_y = view.height / screen_size.y;

#if USE_OPENCL_FOR_DRAW

  cl::Event write_event;

  d_queue_.enqueueWriteBuffer(d_input_buffer_, CL_FALSE, 0,
                              world_.GetElementCount() * sizeof(Element),
                              world_.GetElementsPointer(), nullptr, &write_event);

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
    std::cerr << "Opencl Error : " << error.err() << " : " << error.what() << '\n'
              << "Arg counter : " << arg_counter - 1 << '\n';
    throw;
  }

  cl::Event enqueue_event;
  std::vector<cl::Event> wait_events = {write_event};

  d_queue_.enqueueNDRangeKernel(d_kernel_, cl::NullRange, cl::NDRange(screen_size.x, screen_size.y),
                                cl::NullRange, &wait_events, &enqueue_event);

  cl::Event read_event;
  wait_events = {enqueue_event};

  d_queue_.enqueueReadBuffer(d_output_buffer_, CL_FALSE, 0, GetPixelCount() * sizeof(sf::Color),
                             bytes.data(), &wait_events, &read_event);

  read_event.wait();

#else

  // FORMAT : 0x<A><B><G><R>U
  static const std::array<sf::Color, 9> kColors = {
      sf::Color(0x000000FFU), sf::Color(0xD0D0D0FFU), sf::Color(0x000000FFU),
      sf::Color(0x000000FFU), sf::Color(0x000000FFU), sf::Color(0x000000FFU),
      sf::Color(0x000000FFU), sf::Color(0x000000FFU), sf::Color(0x3B3B38FFU)};

  static const std::array<sf::Color, 4> kSandColors = {
      sf::Color(0xD8A969FFU), sf::Color(0xEABC7AFFU), sf::Color(0xF4C790FFU),
      sf::Color(0xEABC7AFFU)};
  static const std::array<sf::Color, 4> kStoneColors = {
      sf::Color(0x636365FFU), sf::Color(0x66676CFFU), sf::Color(0x545253FFU),
      sf::Color(0x646466FFU)};
  static const std::array<sf::Color, 4> kWaterColors = {
      sf::Color(0x042898FFU), sf::Color(0x0531AAFFU), sf::Color(0x052A9DFFU),
      sf::Color(0x032590FFU)};
  static const std::array<sf::Color, 4> kOilColors = {
      sf::Color(0xCBAA3FFFU), sf::Color(0xC9A73BFFU), sf::Color(0xC8A436FFU),
      sf::Color(0xC19F33FFU)};
  static const std::array<sf::Color, 4> kSteamColors = {
      sf::Color(0xDCDFE4FFU), sf::Color(0xE1E3E8FFU), sf::Color(0xE5E8EDFFU),
      sf::Color(0xD7DADFFFU)};
  static const std::array<sf::Color, 4> kFireColors = {
      sf::Color(0xFF7500FFU), sf::Color(0xFC6400FFU), sf::Color(0xD73502FFU),
      sf::Color(0xB62203FFU)};
  static const std::array<sf::Color, 4> kSmokeColors = {
      sf::Color(0x090909FFU), sf::Color(0x0A0A0AFFU), sf::Color(0x0C0C0CFFU),
      sf::Color(0x0A0A0AFFU)};

  sf::Color* pixel_it = reinterpret_cast<sf::Color*>(bytes.data());

  double world_coord_y = view.top;

  for (uint32_t screen_y = 0; screen_y < screen_size.y; ++screen_y, world_coord_y += step_y) {
    double world_coord_x = view.left;

    for (uint32_t screen_x = 0; screen_x < screen_size.x; ++screen_x, world_coord_x += step_x) {
      const Element element = world_.GetElementAt(
          {static_cast<int32_t>(world_coord_x), static_cast<int32_t>(world_coord_y)});

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

void FallingSandEngine::PlaceElementInLine(const sf::Vector2<int32_t> start_pos,
                                           const sf::Vector2<int32_t> end_pos, const int32_t radius,
                                           const engine::Substance substance) {
  ExecuteInALine(start_pos, end_pos, [&](const float point_on_line_x, const float point_on_line_y) {
    ExecuteInADisc(radius, [&](const int32_t point_on_disc_x, const int32_t point_on_disc_y) {
      world_.SetElementAt(
          ToVector2<int32_t>(sf::Vector2f(point_on_line_x + static_cast<float>(point_on_disc_x),
                                          point_on_line_y + static_cast<float>(point_on_disc_y))),
          Element(substance));
    });
  });
}

void FallingSandEngine::ShowChunkActivity() {
  if (do_show_chunk_activity_) {
    const float chunk_border_size =
        static_cast<float>(constants::kChunkSize * camera_view_.GetZoomLevel());
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

    for (int32_t relative_chunk_y = 0; relative_chunk_y < visible_chunk_count_y;
         ++relative_chunk_y) {
      int32_t consecutive_start = 0;
      bool consecutive_activeness =
          IsChunkActive(top_left_chunk_pos + sf::Vector2<int32_t>{0, relative_chunk_y});

      for (int32_t relative_chunk_x = 1; relative_chunk_x < visible_chunk_count_x;
           ++relative_chunk_x) {
        const sf::Vector2<int32_t> chunk_pos =
            top_left_chunk_pos + sf::Vector2<int32_t>(relative_chunk_x, relative_chunk_y);

        const bool active = IsChunkActive(chunk_pos);

        if (active != consecutive_activeness) {
          const int32_t consecutive_length = relative_chunk_x - consecutive_start;

          rect_shape.setSize(sf::Vector2f(static_cast<float>(consecutive_length), 1) *
                             chunk_border_size);

          const sf::Vector2f screen_chunk_pos =
              ToVector2<float>(sf::Vector2(consecutive_start, relative_chunk_y)) *
              chunk_border_size;

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

        rect_shape.setSize(sf::Vector2f(static_cast<float>(consecutive_length), 1) *
                           chunk_border_size);

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
    const float chunk_border_size =
        static_cast<float>(constants::kChunkSize * camera_view_.GetZoomLevel());
    const sf::Rect<double> fov = camera_view_.GetFieldOfView();

    const sf::Vector2<double> offset = {fov.left - std::fmod(fov.left, constants::kChunkSize),
                                        fov.top - std::fmod(fov.top, constants::kChunkSize)};

    const sf::Vector2f pixel_offset = ToVector2<float>(camera_view_.MapCoordsToPixel(offset));

    const int32_t line_count_x =
        static_cast<int32_t>(static_cast<float>(window_.getSize().x) / chunk_border_size) + 1;
    const int32_t line_count_y =
        static_cast<int32_t>(static_cast<float>(window_.getSize().y) / chunk_border_size) + 1;

    std::array<sf::Vertex, 2> line;

    line[0] = sf::Vertex({pixel_offset.x, 0}, sf::Color(kChunkBorderColor));
    line[1] = sf::Vertex({pixel_offset.x, static_cast<float>(window_.getSize().y)},
                         sf::Color(kChunkBorderColor));

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

  ExecuteInACircle(brush_radius_, [&](const int32_t point_on_circle_x,
                                      const int32_t point_on_circle_y) {
    rect_shape.setPosition(ToVector2<float>(
        pointer_tile + sf::Vector2<int32_t>(point_on_circle_x, point_on_circle_y) * pixel_size));
    window_.draw(rect_shape);
  });
}

void FallingSandEngine::ConstructDebugText(std::string& text_string) const {
  const auto [avg_fps, min_fps] = refresh_rate_.GetFpsInfo();
  const double total_frame_elapsed_time = total_frame_elapsed_time_;
  const double total_frame_percentage =
      total_frame_elapsed_time / constants::kWantedSecondsPerFrame * 100.0;
  const double handle_events_elapsed_percentage =
      handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_percentage =
      draw_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double compute_elapsed_percentage =
      compute_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const uint32_t updated_chunks_count = GetUpdatedChunksCount();

  const sf::Vector2<int32_t> mouse_position =
      ToVector2<int32_t>(window_.mapPixelToCoords(sf::Mouse::getPosition(window_)));
  const sf::Vector2<double> mouse_coord = camera_view_.MapPixelToCoords(mouse_position);
  const sf::Vector2<double> camera_fov = {camera_view_.GetFieldOfView().left,
                                          camera_view_.GetFieldOfView().top};

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
