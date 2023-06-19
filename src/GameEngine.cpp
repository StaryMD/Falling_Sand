#include "GameEngine.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "CameraView.hpp"
#include "CommonUtility.hpp"
#include "FallingSandEngine.hpp"
#include "RefreshRate.hpp"

GameEngine::GameEngine(const std::string& application_name, const unsigned window_width, const unsigned window_height)
    : application_name_(application_name),
      window_(sf::VideoMode(window_width, window_height), application_name_),
      sand_engine_(window_.getSize()),
      camera_view_(window_.getSize(), window_.getSize(), window_.getSize() / 2U) {
  Setup();
}

GameEngine::GameEngine(const std::string& application_name)
    : application_name_(application_name),
      window_(sf::VideoMode::getDesktopMode(), application_name_, sf::Style::Fullscreen),
      sand_engine_(window_.getSize()),
      camera_view_(window_.getSize(), window_.getSize(), window_.getSize() / 2U) {
  Setup();
}

void GameEngine::Setup() {
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
      DrawFrame();
      ComputeNextFrame();

      total_frame_elapsed_time_ = refresh_rate_.GetFrameElapsedTime();
    }
  }
}

void GameEngine::DrawFrame() {
  sf::Clock timer;
  window_.clear(sf::Color(101, 101, 101));  // NOLINT

  sand_engine_.PaintOn(camera_view_, screen_pixels_, this->window_.getSize());
  screen_texture_.update(screen_pixels_.data());
  window_.draw(screen_sprite_);

  ShowDebugInfo();

  window_.display();
  screen_update_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::HandleInput() {
  sf::Clock timer;

  event_handler_.HandleEvents(window_);

  if (event_handler_.GetMouseWheelScrollDelta() != 0) {
    camera_view_.Zoom(event_handler_.GetMouseWheelScrollDelta(), event_handler_.GetMouseScrollWheelLocation());
  }

  if (event_handler_.IsKeyPressed(sf::Keyboard::F3)) {
    do_show_debug_screen_ = !do_show_debug_screen_;
  }

  if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Left)) {
    // const auto drag = event_handler_.GetMouseMovement();

    // ExecuteInALine(drag.first, drag.second, [](const float point_on_line_x, const float point_on_line_y) {
    //   printf("%f %f\n", point_on_line_x, point_on_line_y);
    // });
  }

  if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Middle)) {
    camera_view_.MovePosition(event_handler_.GetMouseMovement().first - event_handler_.GetMouseMovement().second);
  }

  handle_events_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::ComputeNextFrame() {
  sf::Clock timer;

  compute_elapsed_time_ = timer.getElapsedTime().asSeconds();
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
  const double total_frame_percentage = total_frame_elapsed_time_ / kWantedSecondsPerFrame * 100.0;
  // const double handle_events_elapsed_time = handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_time = screen_update_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double compute_elapsed_time = compute_elapsed_time_ / total_frame_elapsed_time * 100.0;

  const sf::Vector2i mouse_position = ToVector2i(window_.mapPixelToCoords(sf::Mouse::getPosition(window_)));
  const sf::Rect<double> camera_fov = camera_view_.GetFieldOfView();

  std::ostringstream debug_string;
  debug_string << std::setprecision(kDebugRealDigitCount) << std::fixed;

  debug_string << "FC: " << refresh_rate_.GetFrameCount() << '\n'
               << '\n'
               << "FPS AVG: " << avg_fps << '\n'
               << "FPS MIN: " << min_fps << '\n'
               << "TOTAL FRAME TIME: " << total_frame_elapsed_time << '\n'
               << "TOTAL FRAME %: " << total_frame_percentage << '\n'
               << '\n'
               //  << "TOTAL EVENT %: " << handle_events_elapsed_time << '\n'
               << "TOTAL DRAW %: " << screen_update_elapsed_time << '\n'
               << "TOTAL COMP %: " << compute_elapsed_time << '\n'
               << '\n'
               << "CAMERA POS : " << camera_fov.left << ' ' << camera_fov.top << '\n'
               << "MOUSE POS : " << mouse_position.x << ' ' << mouse_position.y << '\n';

  return debug_string.str();
}
