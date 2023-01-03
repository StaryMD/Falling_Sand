#pragma once

#include <SFML/Graphics.hpp>

class InputHandler {
public:
	InputHandler();

	bool mouseLEFT_pressed;
	bool mouseRIGHT_pressed;

	bool mouseLEFT_was_pressed;
	bool mouseRIGHT_was_pressed;

	sf::Vector2i mouseLEFT_last_press;
	sf::Vector2i mouseRIGHT_last_press;

	void mouseLEFT_switch();
	void mouseRIGHT_switch();

private:

};
