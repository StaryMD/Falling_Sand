#include "GameEngine.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "CameraView.hpp"
#include "CommonConstants.hpp"
#include "CommonUtility.hpp"
#include "FallingSandEngine.hpp"
#include "RefreshRate.hpp"
#include "world/Substance.hpp"

constexpr uint32_t kChunkColorOpacity = 0x00000033;
constexpr uint32_t kChunkActiveColor = 0x00FF0000 | kChunkColorOpacity;
constexpr uint32_t kChunkInactiveColor = 0xFF000000 | kChunkColorOpacity;

constexpr uint32_t kChunkBorderTransparency = 0x00000033;
constexpr uint32_t kChunkBorderColor = 0x00000000 | kChunkBorderTransparency;

GameEngine::GameEngine(const std::string& application_name)
    : window_(sf::VideoMode::getDesktopMode(), application_name, sf::Style::Fullscreen),
      sand_engine_({constants::kWorldWidth, constants::kWorldHeight}, window_.getSize()),
      camera_view_({constants::kWorldWidth, constants::kWorldHeight}, window_.getSize(),
                   window_.getSize() / 2U) {
  Setup();
}

void GameEngine::Setup() {
  window_.setVerticalSyncEnabled(true);
  screen_texture_.create(window_.getSize().x, window_.getSize().y);
  screen_sprite_.setTexture(screen_texture_);

  const bool font_loaded_successfully =
      font_.loadFromFile(std::filesystem::current_path().string() + "/" + "assets/fonts/consola.ttf");

  if (not font_loaded_successfully) {
    std::cerr << "Font could not be loaded\n";
  }

  screen_pixels_.resize(static_cast<size_t>(window_.getSize().x * window_.getSize().y) * sizeof(sf::Color));

  text_.setFont(font_);
  text_.setCharacterSize(18U);

  refresh_rate_.Reset();
}

void GameEngine::Run() {
  while (window_.isOpen()) {
    if (refresh_rate_.IsTimeForNewFrame()) {
      refresh_rate_.ResetFrameTime();

      HandleInput();

      ComputeNextFrame();
      DrawFrame();

      total_frame_elapsed_time_ = refresh_rate_.GetFrameElapsedTime();
    }
  }
}

void GameEngine::DrawFrame() {
  const sf::Clock timer;

  sand_engine_.PaintOn(camera_view_, screen_pixels_, this->window_.getSize(), tick_count_);
  screen_texture_.update(screen_pixels_.data());
  window_.draw(screen_sprite_);

  ShowChunkActivity();
  ShowChunkBorders();
  ShowDebugInfo();
  DrawBrush();

  window_.display();
  screen_update_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::HandleInput() {
  const sf::Clock timer;

  event_handler_.HandleEvents(window_);

  if (window_.hasFocus()) {
    if (event_handler_.IsKeyPressed(sf::Keyboard::Key::Escape)) {
      window_.close();
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::F2)) {
      do_compute_next_frame_ = !do_compute_next_frame_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::F3)) {
      do_show_debug_screen_ = !do_show_debug_screen_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::F5)) {
      do_show_chunk_borders_ = !do_show_chunk_borders_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::F7)) {
      do_show_chunk_activity_ = !do_show_chunk_activity_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::Space)) {
      if (!do_compute_next_frame_) {
        do_advance_one_frame_ = true;
      }
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::Up)) {
      ++sand_engine_.GetWorld().update_threads_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::Down)) {
      sand_engine_.GetWorld().update_threads_ = std::max(0, sand_engine_.GetWorld().update_threads_ - 1);
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::Left)) {
      ++brush_radius_;
    }

    if (event_handler_.IsKeyPressed(sf::Keyboard::Right)) {
      brush_radius_ = std::max(0, brush_radius_ - 1);
    }

    if (event_handler_.IsKeyDown(sf::Keyboard::Num1)) {
      chosen_substance_ = engine::Substance::kAir;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num2)) {
      chosen_substance_ = engine::Substance::kSand;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num3)) {
      chosen_substance_ = engine::Substance::kStone;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num4)) {
      chosen_substance_ = engine::Substance::kWater;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num5)) {
      chosen_substance_ = engine::Substance::kOil;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num6)) {
      chosen_substance_ = engine::Substance::kSteam;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num7)) {
      chosen_substance_ = engine::Substance::kFire;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num8)) {
      chosen_substance_ = engine::Substance::kSmoke;
    }

    if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Left)) {
      const auto drag = event_handler_.GetMouseMovement();

      sand_engine_.PlaceElementInLine(ToVector2<int32_t>(camera_view_.MapPixelToCoords(drag.first)),
                                      ToVector2<int32_t>(camera_view_.MapPixelToCoords(drag.second)), brush_radius_,
                                      chosen_substance_);
    }

    if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Middle)) {
      camera_view_.MovePosition(event_handler_.GetMouseMovementDelta());
    }

    if (event_handler_.GetMouseWheelScrollDelta() != 0) {
      camera_view_.Zoom(event_handler_.GetMouseWheelScrollDelta(), event_handler_.GetMouseScrollWheelLocation());
    }
  }

  handle_events_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::ComputeNextFrame() {
  const sf::Clock timer;

  if (do_compute_next_frame_ || do_advance_one_frame_) {
    do_advance_one_frame_ = false;
    sand_engine_.Update();
    ++tick_count_;
  }

  compute_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::ShowChunkActivity() {
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

    const int32_t visible_chunk_count_x = static_cast<int32_t>(static_cast<float>(window_.getSize().x) / chunk_border_size) + 2;
    const int32_t visible_chunk_count_y = static_cast<int32_t>(static_cast<float>(window_.getSize().y) / chunk_border_size) + 2;

    for (int32_t relative_chunk_y = 0; relative_chunk_y < visible_chunk_count_y; ++relative_chunk_y) {
      int32_t consecutive_start = 0;
      bool consecutive_activeness = sand_engine_.IsChunkActive(top_left_chunk_pos + sf::Vector2<int32_t>{0, relative_chunk_y});

      for (int32_t relative_chunk_x = 1; relative_chunk_x < visible_chunk_count_x; ++relative_chunk_x) {
        const sf::Vector2<int32_t> chunk_pos = top_left_chunk_pos + sf::Vector2<int32_t>(relative_chunk_x, relative_chunk_y);

        const bool active = sand_engine_.IsChunkActive(chunk_pos);

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

void GameEngine::ShowChunkBorders() {
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

void GameEngine::ShowDebugInfo() {
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

void GameEngine::DrawBrush() {
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

void GameEngine::ConstructDebugText(std::string& text_string) const {
  const auto [avg_fps, min_fps] = refresh_rate_.GetFpsInfo();
  const double total_frame_elapsed_time = total_frame_elapsed_time_;
  const double total_frame_percentage = total_frame_elapsed_time / constants::kWantedSecondsPerFrame * 100.0;
  const double handle_events_elapsed_percentage = handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_percentage = screen_update_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double compute_elapsed_percentage = compute_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const uint32_t updated_chunks_count = sand_engine_.GetUpdatedChunksCount();

  const sf::Vector2<int32_t> mouse_position = ToVector2<int32_t>(window_.mapPixelToCoords(sf::Mouse::getPosition(window_)));
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
               << "UPDATE THREADS : " << sand_engine_.GetWorld().update_threads_ << '\n'
               << "BRUSH RADIUS : " << brush_radius_ << '\n';

  text_string = debug_string.str();
}
