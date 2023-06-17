#include "GameEngine.hpp"

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <utility>

#include <SFML/Graphics.hpp>

#include "CommonUtility.hpp"
#include "RefreshRate.hpp"

GameEngine::GameEngine(const std::string& application_name, const int window_width, const int window_height)
    : application_name_(application_name) {
  window_.create(sf::VideoMode(window_width, window_height), application_name_);
  Setup();
}

GameEngine::GameEngine(const std::string& application_name) : application_name_(application_name) {
  window_.create(sf::VideoMode::getDesktopMode(), application_name_, sf::Style::Fullscreen);
  Setup();
}

void GameEngine::Setup() {
  screen_texture_.create(window_.getSize().x, window_.getSize().y);
  screen_sprite_.setTexture(screen_texture_);

  font_loaded_successfully_ = font_.loadFromFile("assets/fonts/consola.ttf");

  constexpr unsigned kChannelCountPerColor = 4;
  screen_pixels_.resize(static_cast<size_t>(window_.getSize().x * window_.getSize().y) * kChannelCountPerColor);

  text_.setFont(font_);
  text_.setCharacterSize(18U);  // NOLINT

  refresh_rate_.Reset();
}

void GameEngine::Run() {
  while (window_.isOpen()) {
    sf::Clock timer;

    event_handler_.HandleEvents(window_);
    HandleInput();

    handle_events_elapsed_time_ = timer.getElapsedTime().asSeconds();

    if (refresh_rate_.IsTimeForNewFrame()) {
      refresh_rate_.ResetFrameTime();

      Draw();

      total_frame_elapsed_time_ = refresh_rate_.GetFrameElapsedTime();
    }
  }
}

void GameEngine::Draw() {
  sf::Clock timer;
  window_.clear(sf::Color(101, 101, 101));  // NOLINT

  if (do_show_debug_screen_) {
    ShowDebugInfo();
  }

  window_.display();
  screen_update_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::HandleInput() {
  sf::Clock timer;

  if (event_handler_.IsKeyPressed(sf::Keyboard::F3)) {
    do_show_debug_screen_ = !do_show_debug_screen_;
  }

  if (event_handler_.IsMouseButtonDown(sf::Mouse::Button::Left)) {
    // const auto drag = event_handler_.GetMouseMovement();

    // ExecuteInALine(drag.first, drag.second, [](const float point_on_line_x, const float point_on_line_y) {
    //   printf("%f %f\n", point_on_line_x, point_on_line_y);
    // });
  }
}

void GameEngine::ShowDebugInfo() {
  if (font_loaded_successfully_) {
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
  const double handle_events_elapsed_time = handle_events_elapsed_time_ / total_frame_elapsed_time * 100.0;
  const double screen_update_elapsed_time = screen_update_elapsed_time_ / total_frame_elapsed_time * 100.0;

  std::ostringstream debug_string;
  debug_string << std::setprecision(kDebugRealDigitCount) << std::fixed;

  debug_string << "FC: " << refresh_rate_.GetFrameCount() << '\n'
               << "FPS AVG: " << avg_fps << '\n'
               << "FPS MIN: " << min_fps << '\n'
               << "TOTAL FRAME TIME: " << total_frame_elapsed_time << '\n'
               << "TOTAL FRAME %: " << total_frame_percentage << '\n'
               << "TOTAL EVENT %: " << handle_events_elapsed_time << '\n'
               << "TOTAL DRAW  %: " << screen_update_elapsed_time << '\n';

  return debug_string.str();
}