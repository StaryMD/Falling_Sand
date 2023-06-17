#include "InputHandler.hpp"

#include <SFML/System/Vector2.hpp>
#include <utility>

void InputHandler::HandleEvents(sf::RenderWindow& window) {
  sf::Event event;

  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed: {
        window.close();
        break;
      }
      case sf::Event::MouseButtonPressed: {
        SetMouseButtonPressed(event.mouseButton.button, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      } break;
      case sf::Event::MouseButtonReleased: {
        SetMouseButtonReleased(event.mouseButton.button);
      } break;
      case sf::Event::MouseMoved: {
        UpdateMouseLocation(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code != sf::Keyboard::Key::Unknown) {
          SetKeyPressed(event.key.code);
        }
        break;
      }
      case sf::Event::KeyReleased: {
        if (event.key.code != sf::Keyboard::Key::Unknown) {
          SetKeyReleased(event.key.code);
        }
        break;
      }
      default: {
        // NOTHING
      }
    }
  }
}

void InputHandler::SetKeyPressed(const sf::Keyboard::Key key_code) {
  prev_key_press_state_[key_code] = key_press_state_[key_code];
  key_press_state_[key_code] = true;
}

void InputHandler::SetKeyReleased(const sf::Keyboard::Key key_code) {
  prev_key_press_state_[key_code] = key_press_state_[key_code];
  key_press_state_[key_code] = false;
}

void InputHandler::SetMouseButtonPressed(const sf::Mouse::Button mouse_button, const sf::Vector2i location) {
  first_mouse_button_press_location_ = location;
  mouse_button_state_[mouse_button] = true;
}

void InputHandler::SetMouseButtonReleased(const sf::Mouse::Button mouse_button) {
  prev_mouse_button_state_[mouse_button] = mouse_button_state_[mouse_button];
  mouse_button_state_[mouse_button] = false;
}

void InputHandler::UpdateMouseLocation(const sf::Vector2i location) {
  prev_mouse_button_press_location_ = mouse_button_press_location_;
  mouse_button_press_location_ = location;
}

bool InputHandler::IsKeyPressed(const sf::Keyboard::Key key_code) {
  return key_press_state_[key_code];
}

bool InputHandler::IsKeyReleased(const sf::Keyboard::Key key_code) {
  return key_press_state_[key_code] && !prev_key_press_state_[key_code];
}

bool InputHandler::IsMouseButtonPressed(const sf::Mouse::Button mouse_button) {
  return mouse_button_state_[mouse_button];
}

bool InputHandler::IsMouseButtonReleased(const sf::Mouse::Button mouse_button) {
  return mouse_button_state_[mouse_button] && !prev_mouse_button_state_[mouse_button];
}

std::pair<sf::Vector2i, sf::Vector2i> InputHandler::GetMouseMovement() {
  return std::make_pair(prev_mouse_button_press_location_, mouse_button_press_location_);
}
