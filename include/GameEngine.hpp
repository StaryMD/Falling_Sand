#ifndef RENDERING_ENGINE_HPP_
#define RENDERING_ENGINE_HPP_

#include <SFML/Graphics.hpp>

#include "RefreshRate.hpp"
#include "InputHandler.hpp"

class GameEngine {
public:
	GameEngine(const int window_width, const int window_height);

	GameEngine();

	void setup();
	void run();

	void handle_user_input();
	void handle_events();

	void show_debug_info();

	~GameEngine();

private:
	sf::RenderWindow window;
	RefreshRate refresh_rate;
	InputHandler input_handler;

	sf::Event event;

	sf::Texture screen_texture;
	sf::Sprite screen_sprite;
	sf::Uint8* screen_pixels;

	sf::Text fps_text;
	sf::Font font;
};

#endif /* RENDERING_ENGINE_HPP_ */
