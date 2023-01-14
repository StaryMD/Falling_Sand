#include "InputHandler.hpp"

InputHandler::InputHandler() {
	mouseLEFT_is_pressed = false;
	mouseRIGHT_is_pressed = false;
}

void InputHandler::mouseLEFT_switch() {
	mouseLEFT_is_pressed = !mouseLEFT_is_pressed;
}

void InputHandler::mouseRIGHT_switch() {
	mouseRIGHT_is_pressed = !mouseRIGHT_is_pressed;
}

void InputHandler::keyboard_key_switch(const sf::Keyboard::Key key) {
	key_is_pressed[key] = !key_is_pressed[key];
}
