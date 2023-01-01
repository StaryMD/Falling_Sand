#pragma once

#include <SFML/Graphics.hpp>

#include "RefreshRate.hpp"
#include "FallingSandEngine.hpp"
#include "InputHandler.hpp"

class GameEngine {
public:
	GameEngine(FallingSandEngine &fallingSandEngine_, const int window_width, const int window_height);

	GameEngine(FallingSandEngine &fallingSandEngine_);

	void setup();
	void run();

	void handle_user_input();

	void show_fps();

	~GameEngine();

private:
	sf::RenderWindow window;
	FallingSandEngine &fallingSandEngine;
	RefreshRate refresh_rate;
	InputHandler input_handler;

	sf::Event event;

	sf::Texture screen_texture;
	sf::Sprite screen_sprite;
	sf::Uint8* screen_pixels;

	sf::Text fps_text;
	sf::Font font;

};
