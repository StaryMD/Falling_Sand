#include "GameEngine.hpp"

#include <cmath>
#include <cstddef>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <utility>

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

  font_.loadFromFile("fonts/Lato-Regular.ttf");

  screen_pixels_.resize(static_cast<size_t>(window_.getSize().x * window_.getSize().y) * 4U);

  fps_text_.setFillColor(sf::Color::Green);
  fps_text_.setFont(font_);
  fps_text_.setCharacterSize(15U);  // NOLINT

  refresh_rate_.Reset();
}

void GameEngine::Run() {
  while (window_.isOpen()) {
    input_handler_.HandleEvents(window_);

    if (refresh_rate_.GetTimeSinceLastFrame() >= kWantedSecondsPerFrame) {
      refresh_rate_.ResetTimeSinceLastFrame();

      if (input_handler_.IsMouseButtonPressed(sf::Mouse::Button::Left)) {
        const std::pair<sf::Vector2i, sf::Vector2i> drag = input_handler_.GetMouseMovement();

        ExecuteInALine(drag.first, drag.second, [](const float point_on_line_x, const float point_on_line_y) {
          printf("%f %f\n", point_on_line_x, point_on_line_y);
        });
      }
    }
  }
}

void GameEngine::ShowDebugInfo() {
  const auto [avg_fps, min_fps] = refresh_rate_.GetFpsInfo();

  fps_text_.setString(std::to_string(static_cast<int>(std::round(avg_fps))) + " " +
                      std::to_string(static_cast<int>(std::round(min_fps))));

  window_.draw(fps_text_);
}
