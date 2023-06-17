#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <SFML/Window/WindowHandle.hpp>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

class InputHandler {
 public:
  InputHandler() {
    key_press_state_.fill(false);
    prev_key_press_state_.fill(false);
    mouse_button_state_.fill(false);
    prev_mouse_button_state_.fill(false);
  }

  void HandleEvents(sf::RenderWindow& window);

  void SetKeyPressed(sf::Keyboard::Key key_code);
  void SetKeyReleased(sf::Keyboard::Key key_code);

  void SetMouseButtonPressed(sf::Mouse::Button mouse_button, sf::Vector2i location);
  void SetMouseButtonReleased(sf::Mouse::Button mouse_button);
  void UpdateMouseLocation(sf::Vector2i location);

  [[nodiscard]] bool IsKeyPressed(sf::Keyboard::Key key_code);
  [[nodiscard]] bool IsKeyReleased(sf::Keyboard::Key key_code);

  [[nodiscard]] bool IsMouseButtonPressed(sf::Mouse::Button mouse_button);
  [[nodiscard]] bool IsMouseButtonReleased(sf::Mouse::Button mouse_button);

  [[nodiscard]] std::pair<sf::Vector2i, sf::Vector2i> GetMouseMovement();

 private:
  sf::Vector2i mouse_button_press_location_;
  sf::Vector2i prev_mouse_button_press_location_;
  sf::Vector2i first_mouse_button_press_location_;

  std::array<bool, sf::Keyboard::KeyCount> key_press_state_;
  std::array<bool, sf::Keyboard::KeyCount> prev_key_press_state_;

  std::array<bool, sf::Mouse::ButtonCount> mouse_button_state_;
  std::array<bool, sf::Mouse::ButtonCount> prev_mouse_button_state_;
};

#endif /* INPUT_HANDLER_HPP_ */
