#pragma once

#include <SFML/Graphics.hpp>

class InputHandler {
public:
	InputHandler();

	bool mouseLEFT_is_pressed;
	bool mouseRIGHT_is_pressed;

	bool mouseLEFT_was_pressed;
	bool mouseRIGHT_was_pressed;

	sf::Vector2i mouseLEFT_last_press;
	sf::Vector2i mouseRIGHT_last_press;

	bool key_is_pressed[sf::Keyboard::KeyCount];

	void mouseLEFT_switch();
	void mouseRIGHT_switch();

	void keyboard_key_switch(const sf::Keyboard::Key key);

private:

};
