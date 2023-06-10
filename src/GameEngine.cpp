#include "GameEngine.hpp"

#include <cmath>

GameEngine::GameEngine(const std::string& application_name, const int window_width, const int window_height)
    : application_name_(application_name) {
  window_.create(sf::VideoMode(window_width, window_height), application_name_);
  setup();
}

GameEngine::GameEngine(const std::string& application_name) : application_name_(application_name) {
  window_.create(sf::VideoMode::getDesktopMode(), application_name_, sf::Style::Fullscreen);
  setup();
}

void GameEngine::setup() {
  screen_texture_.create(window_.getSize().x, window_.getSize().y);
  screen_sprite_.setTexture(screen_texture_);

  font_.loadFromFile("fonts/Lato-Regular.ttf");

  screen_pixels_.resize(window_.getSize().x * window_.getSize().y * 4);

  refresh_rate_.reset();

  fps_text_.setFillColor(sf::Color::Green);
  fps_text_.setFont(font_);
  fps_text_.setCharacterSize(15);
}

void GameEngine::run() {
  while (window_.isOpen()) {
    handle_events();

    if (window_.hasFocus()) {
      handle_user_input();
    }

    if (refresh_rate_.get_time_since_last_frame() >= WANTED_SECONDS_PER_FRAME) {
      refresh_rate_.reset_time_since_last_frame();
    }
  }
}

void GameEngine::handle_events() {
  while (window_.pollEvent(event_)) {
    switch (event_.type) {
      case sf::Event::Closed: {
        window_.close();
        break;
      }
      case sf::Event::MouseButtonPressed: {
        switch (event_.mouseButton.button) {
          case sf::Mouse::Left:
            input_handler_.mouseLEFT_is_pressed_ = true;
            break;
          case sf::Mouse::Right:
            input_handler_.mouseRIGHT_is_pressed_ = true;
            break;
          default: {
            // NOTHING
          }
        }
        break;
      }
      case sf::Event::MouseButtonReleased: {
        switch (event_.mouseButton.button) {
          case sf::Mouse::Left:
            input_handler_.mouseLEFT_is_pressed_ = false;
            break;
          case sf::Mouse::Right:
            input_handler_.mouseRIGHT_is_pressed_ = false;
            break;
          default: {
            // NOTHING
          }
        }
        break;
      }
      case sf::Event::KeyPressed: {
        if (event_.key.code != sf::Keyboard::Key::Unknown) {
          input_handler_.key_is_pressed_[event_.key.code] = true;
        }
        break;
      }
      case sf::Event::KeyReleased: {
        if (event_.key.code != sf::Keyboard::Key::Unknown) {
          input_handler_.key_is_pressed_[event_.key.code] = false;
        }
        break;
      }
      default: {
        // NOTHING
      }
    }
  }
}

void GameEngine::handle_user_input() {
  if (input_handler_.mouseLEFT_is_pressed_) {
    const sf::Vector2i cur_position = sf::Mouse::getPosition(window_);

    input_handler_.mouseLEFT_last_press_ = cur_position;
  }

  input_handler_.mouseLEFT_was_pressed_ = input_handler_.mouseLEFT_is_pressed_;
  input_handler_.mouseRIGHT_was_pressed_ = input_handler_.mouseRIGHT_is_pressed_;
}

void GameEngine::show_debug_info() {
  const auto [avg_fps, min_fps] = refresh_rate_.get_fps_info();

  fps_text_.setString(std::to_string((int)std::round(avg_fps)) + " " + std::to_string((int)std::round(min_fps)));

  window_.draw(fps_text_);
}
