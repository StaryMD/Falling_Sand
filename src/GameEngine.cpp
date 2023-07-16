#include "GameEngine.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "CameraView.hpp"
#include "CommonConstants.hpp"
#include "CommonUtility.hpp"
#include "FallingSandEngine.hpp"
#include "RefreshRate.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

GameEngine::GameEngine(const std::string& application_name)
    : application_name_(application_name),
      window_(sf::VideoMode::getDesktopMode(), application_name_, sf::Style::Fullscreen),
      sand_engine_(sf::Vector2u(constants::kWorldWidth, constants::kWorldHeight), window_.getSize()),
      camera_view_(sf::Vector2u(constants::kWorldWidth, constants::kWorldHeight), window_.getSize(),
                   window_.getSize() / 2U) {
  Setup();
}

void GameEngine::Setup() {
  window_.setVerticalSyncEnabled(true);
  screen_texture_.create(window_.getSize().x, window_.getSize().y);
  screen_sprite_.setTexture(screen_texture_);

  font_loaded_successfully_ = font_.loadFromFile("assets/fonts/consola.ttf");

  screen_pixels_.resize(static_cast<size_t>(window_.getSize().x * window_.getSize().y) * sizeof(sf::Color));

  text_.setFont(font_);
  text_.setCharacterSize(18U);  // NOLINT

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
  sf::Clock timer;

  sand_engine_.PaintOn(camera_view_, screen_pixels_, this->window_.getSize());
  screen_texture_.update(screen_pixels_.data());
  window_.draw(screen_sprite_);

  ShowChunkBorders();
  ShowDebugInfo();

  window_.display();
  screen_update_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::HandleInput() {
  sf::Clock timer;

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

    if (event_handler_.IsKeyPressed(sf::Keyboard::Space)) {
      if (!do_compute_next_frame_) {
        do_advance_one_frame_ = true;
      }
    }

    if (event_handler_.IsKeyDown(sf::Keyboard::Num1)) {
      chosen_substance_ = engine::Substance::kAir;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num2)) {
      chosen_substance_ = engine::Substance::kSand;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num3)) {
      chosen_substance_ = engine::Substance::kStone;
    } else if (event_handler_.IsKeyDown(sf::Keyboard::Num4)) {
      chosen_substance_ = engine::Substance::kWater;
    }

    if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Left)) {
      const auto drag = event_handler_.GetMouseMovement();

      sand_engine_.PlaceElementInLine(ToVector2<int>(camera_view_.MapPixelToCoords(drag.first)),
                                      ToVector2<int>(camera_view_.MapPixelToCoords(drag.second)), chosen_substance_);
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
  sf::Clock timer;

  if (do_compute_next_frame_ || do_advance_one_frame_) {
    do_advance_one_frame_ = false;
    sand_engine_.Update();
  }

  compute_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::ShowChunkBorders() {
  if (do_show_chunk_borders_) {
    const float chunk_border_size = static_cast<float>(constants::kChunkSize * camera_view_.GetZoomLevel());
    const sf::Rect<double> fov = camera_view_.GetFieldOfView();

    const sf::Vector2<double> offset = {fov.left - std::fmod(fov.left, constants::kChunkSize),
                                        fov.top - std::fmod(fov.top, constants::kChunkSize)};

    const sf::Vector2f pixel_offset = ToVector2<float>(camera_view_.MapCoordsToPixel(offset));

    const int line_count_x = static_cast<int>(static_cast<float>(window_.getSize().x) / chunk_border_size) + 1;
    const int line_count_y = static_cast<int>(static_cast<float>(window_.getSize().y) / chunk_border_size) + 1;

    std::array<sf::Vertex, 2> line;

    constexpr int kChunkBorderTransparency = 192;
    const sf::Color chunk_border_color = sf::Color(0, 0, 0, kChunkBorderTransparency);

    line[0] = sf::Vertex({pixel_offset.x, 0}, chunk_border_color);
    line[1] = sf::Vertex({pixel_offset.x, static_cast<float>(window_.getSize().y)}, chunk_border_color);

    for (int i = 0; i < line_count_x; ++i) {
      line[0].position.x += chunk_border_size;
      line[1].position.x += chunk_border_size;

      window_.draw(line.data(), 2, sf::Lines);
    }

    line[0] = sf::Vertex({0, pixel_offset.y}, sf::Color(0, 0, 0, kChunkBorderTransparency));
    line[1] = sf::Vertex({static_cast<float>(window_.getSize().x), pixel_offset.y},
                         sf::Color(0, 0, 0, kChunkBorderTransparency));

    for (int i = 0; i < line_count_y; ++i) {
      line[0].position.y += chunk_border_size;
      line[1].position.y += chunk_border_size;

      window_.draw(line.data(), 2, sf::Lines);
    }
  }
}

void GameEngine::ShowDebugInfo() {
  if (do_show_debug_screen_ && font_loaded_successfully_) {
    text_.setString(ConstructDebugText());

    text_.setFillColor(sf::Color::Black);
    text_.setPosition(3, 3);
    window_.draw(text_);

    text_.setFillColor(sf::Color::White);
    text_.setPosition(1, 1);
    window_.draw(text_);
  }
}

std::string GameEngine::ConstructDebugText() const {
  const auto [avg_fps, min_fps] = refresh_rate_.GetFpsInfo();
  const double total_frame_elapsed_time = total_frame_elapsed_time_;
  const double total_frame_percentage = total_frame_elapsed_time_ / constants::kWantedSecondsPerFrame * 100.0;
  const double handle_events_elapsed_time = handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_time = screen_update_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double compute_elapsed_time = compute_elapsed_time_ / total_frame_elapsed_time * 100.0;

  const sf::Vector2i mouse_position = ToVector2<int>(window_.mapPixelToCoords(sf::Mouse::getPosition(window_)));
  const sf::Vector2<double> mouse_coord = camera_view_.MapPixelToCoords(mouse_position);
  const sf::Rect<double> camera_fov = camera_view_.GetFieldOfView();

  std::ostringstream debug_string;
  debug_string << std::setprecision(constants::kDebugDigitPrecision) << std::fixed
               << "FC: " << refresh_rate_.GetFrameCount() << '\n'
               << '\n'
               << "FPS AVG: " << avg_fps << '\n'
               << "FPS MIN: " << min_fps << '\n'
               << "TOTAL FRAME TIME: " << total_frame_elapsed_time << '\n'
               << "TOTAL FRAME %: " << total_frame_percentage << '\n'
               << '\n'
               << "TOTAL EVENT %: " << handle_events_elapsed_time << '\n'
               << "TOTAL COMP %: " << compute_elapsed_time << '\n'
               << "TOTAL DRAW %: " << screen_update_elapsed_time << '\n'
               << '\n'
               << "MOUSE PIXEL : " << mouse_position.x << ' ' << mouse_position.y << '\n'
               << "CAMERA POS : " << camera_fov.left << ' ' << camera_fov.top << '\n'
               << "MOUSE COORD : " << mouse_coord.x << ' ' << mouse_coord.y << '\n';

  return debug_string.str();
}
