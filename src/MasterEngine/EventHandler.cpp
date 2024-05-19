#include "MasterEngine/EventHandler.hpp"

#include <cstdint>
#include <utility>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

void EventHandler::HandleEvents(sf::RenderWindow& window) {
  for (int32_t i = static_cast<int32_t>(keys_to_update_.size()) - 1; i >= 0; --i) {
    key_was_down_[keys_to_update_[i]] = key_is_down_[keys_to_update_[i]];
    keys_to_update_.pop_back();
  }

  for (int32_t i = static_cast<int32_t>(mouse_buttons_to_update_.size()) - 1; i >= 0; --i) {
    mouse_button_was_down_[mouse_buttons_to_update_[i]] = mouse_button_is_down_[mouse_buttons_to_update_[i]];
    mouse_buttons_to_update_.pop_back();
  }

  mouse_wheel_scroll_delta_ = 0;
  UpdateMouseLocation();

  for (sf::Event event; window.pollEvent(event);) {
    switch (event.type) {
      case sf::Event::Closed: {
        window.close();
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code != sf::Keyboard::Key::Unknown) {
          SetKeyDown(event.key.code);
          keys_to_update_.push_back(event.key.code);
        }
        break;
      }
      case sf::Event::KeyReleased: {
        if (event.key.code != sf::Keyboard::Key::Unknown) {
          SetKeyUp(event.key.code);
          keys_to_update_.push_back(event.key.code);
        }
        break;
      }
      case sf::Event::MouseButtonPressed: {
        SetMouseButtonUp(event.mouseButton.button);
        mouse_buttons_to_update_.push_back(event.mouseButton.button);
      } break;
      case sf::Event::MouseButtonReleased: {
        SetMouseButtonDown(event.mouseButton.button);
        mouse_buttons_to_update_.push_back(event.mouseButton.button);
      } break;
      case sf::Event::MouseWheelScrolled: {
        mouse_wheel_scroll_delta_ = event.mouseWheelScroll.delta;
        mouse_wheel_scroll_location_ = {event.mouseWheelScroll.x, event.mouseWheelScroll.y};
        break;
      }
      default: {
        // NOTHING
      }
    }
  }
}

// KEYBOARD

void EventHandler::SetKeyDown(const sf::Keyboard::Key key_code) {
  key_was_down_[key_code] = key_is_down_[key_code];
  key_is_down_[key_code] = true;
}

void EventHandler::SetKeyUp(const sf::Keyboard::Key key_code) {
  key_was_down_[key_code] = key_is_down_[key_code];
  key_is_down_[key_code] = false;
}

bool EventHandler::IsKeyDown(const sf::Keyboard::Key key_code) const {
  return key_is_down_[key_code];
}

bool EventHandler::IsKeyUp(const sf::Keyboard::Key key_code) const {
  return !IsKeyDown(key_code);
}

bool EventHandler::IsKeyPressed(const sf::Keyboard::Key key_code) const {
  return key_is_down_[key_code] && !key_was_down_[key_code];
}

bool EventHandler::IsKeyReleased(const sf::Keyboard::Key key_code) const {
  return !key_is_down_[key_code] && key_was_down_[key_code];
}

// MOUSE

void EventHandler::SetMouseButtonUp(const sf::Mouse::Button mouse_button) {
  mouse_button_was_down_[mouse_button] = mouse_button_is_down_[mouse_button];
  mouse_button_is_down_[mouse_button] = true;
}

void EventHandler::SetMouseButtonDown(const sf::Mouse::Button mouse_button) {
  mouse_button_was_down_[mouse_button] = mouse_button_is_down_[mouse_button];
  mouse_button_is_down_[mouse_button] = false;
}

bool EventHandler::IsMouseButtonDown(const sf::Mouse::Button mouse_button) const {
  return mouse_button_is_down_[mouse_button];
}

bool EventHandler::IsMouseButtonUp(const sf::Mouse::Button mouse_button) const {
  return !IsMouseButtonDown(mouse_button);
}

bool EventHandler::IsMouseButtonPressed(sf::Keyboard::Key mouse_button) const {
  return mouse_button_is_down_[mouse_button] && !mouse_button_was_down_[mouse_button];
}

bool EventHandler::IsMouseButtonReleased(sf::Keyboard::Key mouse_button) const {
  return !mouse_button_is_down_[mouse_button] && mouse_button_was_down_[mouse_button];
}

void EventHandler::UpdateMouseLocation() {
  prev_mouse_location_ = mouse_location_;
  mouse_location_ = sf::Mouse::getPosition();
}

std::pair<sf::Vector2<int32_t>, sf::Vector2<int32_t>> EventHandler::GetMouseMovement() const {
  return std::make_pair(prev_mouse_location_, mouse_location_);
}

float EventHandler::GetMouseWheelScrollDelta() const {
  return mouse_wheel_scroll_delta_;
}

sf::Vector2<int32_t> EventHandler::GetMouseScrollWheelLocation() const {
  return mouse_wheel_scroll_location_;
}

sf::Vector2<int32_t> EventHandler::GetMouseMovementDelta() const {
  return prev_mouse_location_ - mouse_location_;
}

sf::Vector2<int32_t> EventHandler::GetMousePosition() const {
  return mouse_location_;
}
