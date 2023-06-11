#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <array>

#include <SFML/Graphics.hpp>

class InputHandler {
 public:
  InputHandler();

 private:
  bool mouseLEFT_is_pressed_;
  bool mouseRIGHT_is_pressed_;

  bool mouseLEFT_was_pressed_;
  bool mouseRIGHT_was_pressed_;

  sf::Vector2i mouseLEFT_last_press_;
  sf::Vector2i mouseRIGHT_last_press_;

  std::array<bool, sf::Keyboard::KeyCount> key_is_pressed_;
};

#endif /* INPUT_HANDLER_HPP_ */
