#include "GameEngine.hpp"
#include "FallingSandEngine.hpp"

#include <cmath>

GameEngine::GameEngine(FallingSandEngine &fallingSandEngine_, const int window_width, const int window_height)
	: fallingSandEngine(fallingSandEngine_) {
	window.create(sf::VideoMode(window_width, window_height), "Falling Sand");
	setup();
}

GameEngine::GameEngine(FallingSandEngine &fallingSandEngine_)
	: fallingSandEngine(fallingSandEngine_) {
	window.create(sf::VideoMode::getDesktopMode(), "Falling Sand", sf::Style::Fullscreen);
	setup();
}

void GameEngine::setup() {
	screen_texture.create(window.getSize().x, window.getSize().y);
	screen_sprite.setTexture(screen_texture);

	font.loadFromFile("fonts/Lato-Regular.ttf");

	screen_pixels = new sf::Uint8[window.getSize().x * window.getSize().y * 4];

	refresh_rate.setup();

	fps_text.setFillColor(sf::Color::Green);
	fps_text.setFont(font);
	fps_text.setCharacterSize(15);
}

void GameEngine::run() {
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			#pragma GCC diagnostic ignored "-Wswitch"
			switch (event.type) {
				case sf::Event::Closed: {
					window.close();
					break;
				}
				case sf::Event::MouseButtonPressed:
				case sf::Event::MouseButtonReleased: {
					switch (event.mouseButton.button) {
						case sf::Mouse::Left:
							input_handler.mouseLEFT_switch();
							break;
						case sf::Mouse::Right:
							input_handler.mouseRIGHT_switch();
							break;
					}
					break;
				}
			}
		}
		
		handle_user_input();

		if (refresh_rate.get_time_since_last_frame() >= WANTED_SECONDS_PER_FRAME) {
			refresh_rate.reset_time_since_last_frame();

			fallingSandEngine.draw_world_on_texture(screen_pixels);
			screen_texture.update(screen_pixels);

			window.draw(screen_sprite);
			show_fps();
			window.display();

			fallingSandEngine.update();
		}
	}
}

void GameEngine::handle_user_input() {
	if (window.hasFocus()) {
		if (input_handler.mouseLEFT_pressed) {
			const sf::Vector2i cur_position = sf::Mouse::getPosition(window);
			
			fallingSandEngine.set_cell(cur_position.y, cur_position.x, Substance::SAND);
		}
	}
}

void GameEngine::show_fps() {
	const auto [avg_fps, min_fps] = refresh_rate.get_fps_info();

	fps_text.setString(std::to_string((int) std::round(avg_fps)) + " " + std::to_string((int) std::round(min_fps)));

	window.draw(fps_text);
}

GameEngine::~GameEngine() {
	delete[] screen_pixels;
}
