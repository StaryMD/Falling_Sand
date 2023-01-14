#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "RefreshRate.hpp"
#include "FallingSandEngine.hpp"
#include "InputHandler.hpp"

class GameEngine {
public:
	GameEngine(const int window_width, const int window_height);

	GameEngine();

	void setup();
	void run();

	void handle_user_input();
	void handle_events();

	void show_fps();

	void set_line_of_elements(const sf::Vector2i &pos1, const sf::Vector2i &pos2, const Element element);

	~GameEngine();

private:
	sf::RenderWindow window;
	FallingSandEngine fallingSandEngine;
	RefreshRate refresh_rate;
	InputHandler input_handler;

	sf::Event event;

	sf::Texture screen_texture;
	sf::Sprite screen_sprite;
	sf::Uint8* screen_pixels;

	sf::Text fps_text;
	sf::Font font;

	Substance chosen_substance;

};
