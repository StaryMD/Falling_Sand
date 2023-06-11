#include "GameEngine.hpp"

#include <cmath>
#include <cstddef>
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

  refresh_rate_.Reset();

  fps_text_.setFillColor(sf::Color::Green);
  fps_text_.setFont(font_);
  fps_text_.setCharacterSize(15U);  // NOLINT
}

void GameEngine::Run() {
  while (window_.isOpen()) {
    HandleEvents();

    if (refresh_rate_.GetTimeSinceLastFrame() >= kWantedSecondsPerFrame) {
      refresh_rate_.ResetTimeSinceLastFrame();
    }
  }
}

void GameEngine::HandleEvents() {
  while (window_.pollEvent(event_)) {
    switch (event_.type) {
      case sf::Event::Closed: {
        window_.close();
        break;
      }
        // case sf::Event::MouseButtonPressed: {
        //   switch (event_.mouseButton.button) {
        //     case sf::Mouse::Left:

        //       break;
        //     case sf::Mouse::Right:

        //       break;
        //     default: {
        //       // NOTHING
        //     }
        //   }
        //   break;
        // }
        // case sf::Event::MouseButtonReleased: {
        //   switch (event_.mouseButton.button) {
        //     case sf::Mouse::Left:

        //       break;
        //     case sf::Mouse::Right:

        //       break;
        //     default: {
        //       // NOTHING
        //     }
        //   }
        //   break;
        // }
        // case sf::Event::KeyPressed: {
        //   if (event_.key.code != sf::Keyboard::Key::Unknown) {

        //   }
        //   break;
        // }
        // case sf::Event::KeyReleased: {
        //   if (event_.key.code != sf::Keyboard::Key::Unknown) {

        //   }
        //   break;
        // }
        // default: {
        //   // NOTHING
        // }
      default: {
        // NOTHING
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
