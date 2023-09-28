#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowHandle.hpp>

class EventHandler {
 public:
  EventHandler() {
    key_is_down_.fill(false);
    key_was_down_.fill(false);
    mouse_button_is_down_.fill(false);
    mouse_button_was_down_.fill(false);
  }

  void HandleEvents(sf::RenderWindow& window);

  [[nodiscard]] bool IsKeyDown(sf::Keyboard::Key key_code) const;
  [[nodiscard]] bool IsKeyUp(sf::Keyboard::Key key_code) const;

  [[nodiscard]] bool IsKeyPressed(sf::Keyboard::Key key_code) const;
  [[nodiscard]] bool IsKeyReleased(sf::Keyboard::Key key_code) const;

  [[nodiscard]] bool IsMouseButtonDown(sf::Mouse::Button mouse_button) const;
  [[nodiscard]] bool IsMouseButtonUp(sf::Mouse::Button mouse_button) const;

  [[nodiscard]] bool IsMouseButtonPressed(sf::Keyboard::Key mouse_button) const;
  [[nodiscard]] bool IsMouseButtonReleased(sf::Keyboard::Key mouse_button) const;

  [[nodiscard]] float GetMouseWheelScrollDelta() const;

  [[nodiscard]] sf::Vector2i GetMouseScrollWheelLocation() const;

  [[nodiscard]] std::pair<sf::Vector2i, sf::Vector2i> GetMouseMovement() const;

  [[nodiscard]] sf::Vector2i GetMouseMovementDelta() const;

 private:
  void SetKeyDown(sf::Keyboard::Key key_code);
  void SetKeyUp(sf::Keyboard::Key key_code);

  void SetMouseButtonUp(sf::Mouse::Button mouse_button);
  void SetMouseButtonDown(sf::Mouse::Button mouse_button);
  void UpdateMouseLocation();

  sf::Vector2i mouse_location_;
  sf::Vector2i prev_mouse_location_;

  sf::Vector2i mouse_wheel_scroll_location_;
  float mouse_wheel_scroll_delta_;

  std::array<bool, sf::Keyboard::KeyCount> key_is_down_;
  std::array<bool, sf::Keyboard::KeyCount> key_was_down_;

  std::array<bool, sf::Mouse::ButtonCount> mouse_button_is_down_;
  std::array<bool, sf::Mouse::ButtonCount> mouse_button_was_down_;

  std::vector<sf::Keyboard::Key> keys_to_update_;
  std::vector<sf::Mouse::Button> mouse_buttons_to_update_;
};

#endif /* INPUT_HANDLER_HPP_ */
