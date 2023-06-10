#include "InputHandler.hpp"

InputHandler::InputHandler() {
  mouseLEFT_is_pressed_ = false;
  mouseRIGHT_is_pressed_ = false;
}

void InputHandler::mouseLEFT_switch() {
  mouseLEFT_is_pressed_ = !mouseLEFT_is_pressed_;
}

void InputHandler::mouseRIGHT_switch() {
  mouseRIGHT_is_pressed_ = !mouseRIGHT_is_pressed_;
}
