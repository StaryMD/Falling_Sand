#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

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

private:

};

#endif /* INPUT_HANDLER_HPP_ */
