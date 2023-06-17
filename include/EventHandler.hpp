#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowHandle.hpp>

#include "RefreshRate.hpp"

class EventHandler {
 public:
  EventHandler() {
    key_is_down_.fill(false);
    key_was_down_.fill(false);
    mouse_button_is_down_.fill(false);
    mouse_button_was_down_.fill(false);
  }

  void HandleEvents(sf::RenderWindow& window);

  [[nodiscard]] bool IsKeyDown(sf::Keyboard::Key key_code);
  [[nodiscard]] bool IsKeyUp(sf::Keyboard::Key key_code);

  [[nodiscard]] bool IsKeyPressed(sf::Keyboard::Key key_code);
  [[nodiscard]] bool IsKeyReleased(sf::Keyboard::Key key_code);

  [[nodiscard]] bool IsMouseButtonDown(sf::Mouse::Button mouse_button);
  [[nodiscard]] bool IsMouseButtonUp(sf::Mouse::Button mouse_button);

  [[nodiscard]] bool IsMouseButtonPressed(sf::Keyboard::Key mouse_button);
  [[nodiscard]] bool IsMouseButtonReleased(sf::Keyboard::Key mouse_button);

  [[nodiscard]] std::pair<sf::Vector2i, sf::Vector2i> GetMouseMovement();

 private:
  void SetKeyDown(sf::Keyboard::Key key_code);
  void SetKeyUp(sf::Keyboard::Key key_code);

  void SetMouseButtonUp(sf::Mouse::Button mouse_button);
  void SetMouseButtonDown(sf::Mouse::Button mouse_button);
  void UpdateMouseLocation(sf::Vector2i location);

  sf::Vector2i mouse_location_;
  sf::Vector2i prev_mouse_location_;

  std::array<bool, sf::Keyboard::KeyCount> key_is_down_;
  std::array<bool, sf::Keyboard::KeyCount> key_was_down_;

  std::array<bool, sf::Mouse::ButtonCount> mouse_button_is_down_;
  std::array<bool, sf::Mouse::ButtonCount> mouse_button_was_down_;

  std::vector<sf::Keyboard::Key> keys_to_update_;
  std::vector<sf::Keyboard::Key> mouse_buttons_to_update_;
};

#endif /* INPUT_HANDLER_HPP_ */
