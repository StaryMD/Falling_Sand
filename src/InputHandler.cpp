#include "InputHandler.hpp"

InputHandler::InputHandler() {
	mouseLEFT_pressed = false;
	mouseRIGHT_pressed = false;
}

void InputHandler::mouseLEFT_switch() {
	mouseLEFT_pressed = !mouseLEFT_pressed;
}

void InputHandler::mouseRIGHT_switch() {
	mouseRIGHT_pressed = !mouseRIGHT_pressed;
}
