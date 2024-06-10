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

  /**
   * @brief Handle all input-related events from a window.
   * 
   * @param window Window to handle events for.
   */
  void HandleEvents(sf::RenderWindow& window);

  /**
   * @brief Check whether a key is down.
   * 
   * @param key_code Code of the key to check if is down.
   * @return bool Whether the key is down.
   */
  [[nodiscard]] bool IsKeyDown(sf::Keyboard::Key key_code) const;

  /**
   * @brief Check whether a key is up.
   * 
   * @param key_code Code of the key to check if is up.
   * @return bool Whether the key is up.
   */
  [[nodiscard]] bool IsKeyUp(sf::Keyboard::Key key_code) const;

  /**
   * @brief Check whether a key has been pressed.
   * 
   * @details Will use the state from the previous frame to calculate the value.
   * @param key_code Code of the key to check if has been pressed.
   * @return bool Whether the key has been pressed.
   */
  [[nodiscard]] bool IsKeyPressed(sf::Keyboard::Key key_code) const;

  /**
   * @brief Check whether a key has been released.
   * 
   * @details Will use the state from the previous frame to calculate the value.
   * @param key_code Code of the key to check if has been released.
   * @return bool Whether the key has been released.
   */
  [[nodiscard]] bool IsKeyReleased(sf::Keyboard::Key key_code) const;

  /**
   * @brief Check whether a mouse button is down.
   * 
   * @param mouse_button Code of the mouse button to check if is down.
   * @return bool Whether the mouse button is down.
   */
  [[nodiscard]] bool IsMouseButtonDown(sf::Mouse::Button mouse_button) const;

  /**
   * @brief Check whether a mouse button is up.
   * 
   * @param mouse_button Code of the mouse button to check if is up.
   * @return bool Whether the mouse button is up.
   */
  [[nodiscard]] bool IsMouseButtonUp(sf::Mouse::Button mouse_button) const;

  /**
   * @brief Check whether a mouse button has been pressed.
   * 
   * @details Will use the state from the previous frame to calculate the value.
   * @param mouse_button Code of the mouse button to check if has been pressed.
   * @return bool Whether the mouse button has been pressed.
   */
  [[nodiscard]] bool IsMouseButtonPressed(sf::Keyboard::Key mouse_button) const;

  /**
   * @brief Check whether a mouse button has been released.
   * 
   * @details Will use the state from the previous frame to calculate the value.
   * @param mouse_button Code of the mouse button to check if has been released.
   * @return bool Whether the mouse button has been released.
   */
  [[nodiscard]] bool IsMouseButtonReleased(sf::Keyboard::Key mouse_button) const;

  /**
   * @brief Get the how much the scroll wheel turned this frame.
   * 
   * @return float Values of how much the scrool wheel turned this frame.
   */
  [[nodiscard]] float GetMouseWheelScrollDelta() const;

  /**
   * @brief Get the location of where the scroll wheel turned this frame.
   * 
   * @return sf::Vector2<int32_t> Location of where the scroll wheel turned this frame.
   */
  [[nodiscard]] sf::Vector2<int32_t> GetMouseScrollWheelLocation() const;

  /**
   * @brief Get where the mouse started at the beginning of the frame and where it ended at the end.
   * 
   * @return std::pair<sf::Vector2<int32_t>, sf::Vector2<int32_t>> Starting postion and ending position.
   */
  [[nodiscard]] std::pair<sf::Vector2<int32_t>, sf::Vector2<int32_t>> GetMouseMovement() const;

  /**
   * @brief Get how much the mouse moved this frame.
   * 
   * @return sf::Vector2<int32_t> Vector of how much the mouse moved this frame.
   */
  [[nodiscard]] sf::Vector2<int32_t> GetMouseMovementDelta() const;

  /**
   * @brief Get the current postion of the mouse.
   * 
   * @return sf::Vector2<int32_t> Current position of the mouse.
   */
  [[nodiscard]] sf::Vector2<int32_t> GetMousePosition() const;

 private:
  /**
   * @brief Set a key down in the internal storage array.
   * 
   * @param key_code Code of the key to be set.
   */
  void SetKeyDown(sf::Keyboard::Key key_code);

  /**
   * @brief Set a key up in the internal storage array.
   * 
   * @param key_code Code of the key to be set.
   */
  void SetKeyUp(sf::Keyboard::Key key_code);

  /**
   * @brief Set a mouse button up in the internal storage array.
   * 
   * @param mouse_button Code of the mouse button to be set.
   */
  void SetMouseButtonUp(sf::Mouse::Button mouse_button);

  /**
   * @brief Set a mouse button down in the internal storage array.
   * 
   * @param mouse_button Code of the mouse button to be set.
   */
  void SetMouseButtonDown(sf::Mouse::Button mouse_button);

  /**
   * @brief Update the stored location of the mouse.
   * 
   */
  void UpdateMouseLocation();

  sf::Vector2<int32_t> mouse_location_;
  sf::Vector2<int32_t> prev_mouse_location_;

  sf::Vector2<int32_t> mouse_wheel_scroll_location_;
  float mouse_wheel_scroll_delta_;

  std::array<bool, sf::Keyboard::KeyCount> key_is_down_;
  std::array<bool, sf::Keyboard::KeyCount> key_was_down_;

  std::array<bool, sf::Mouse::ButtonCount> mouse_button_is_down_;
  std::array<bool, sf::Mouse::ButtonCount> mouse_button_was_down_;

  std::vector<sf::Keyboard::Key> keys_to_update_;
  std::vector<sf::Mouse::Button> mouse_buttons_to_update_;
};

#endif  // INPUT_HANDLER_HPP_
