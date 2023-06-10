#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <SFML/Graphics.hpp>

class InputHandler {
 public:
  InputHandler();

  bool mouseLEFT_is_pressed_;
  bool mouseRIGHT_is_pressed_;

  bool mouseLEFT_was_pressed_;
  bool mouseRIGHT_was_pressed_;

  sf::Vector2i mouseLEFT_last_press_;
  sf::Vector2i mouseRIGHT_last_press_;

  bool key_is_pressed_[sf::Keyboard::KeyCount];

  void mouseLEFT_switch();
  void mouseRIGHT_switch();
};

#endif /* INPUT_HANDLER_HPP_ */
