#include "GameEngine.hpp"

#include <cmath>

GameEngine::GameEngine(const int window_width, const int window_height) {
	window.create(sf::VideoMode(window_width, window_height), "Falling Sand");
	setup();
}

GameEngine::GameEngine() {
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
		handle_events();
		
		if (window.hasFocus()) {
			handle_user_input();
		}

		if (refresh_rate.get_time_since_last_frame() >= WANTED_SECONDS_PER_FRAME) {
			refresh_rate.reset_time_since_last_frame();
			

		}
	}
}

void GameEngine::handle_events() {
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed: {
				window.close();
				break;
			}
			case sf::Event::MouseButtonPressed: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Left:
						input_handler.mouseLEFT_is_pressed = true;
						break;
					case sf::Mouse::Right:
						input_handler.mouseRIGHT_is_pressed = true;
						break;
					default: {
						// NOTHING
					}
				}
				break;
			}
			case sf::Event::MouseButtonReleased: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Left:
						input_handler.mouseLEFT_is_pressed = false;
						break;
					case sf::Mouse::Right:
						input_handler.mouseRIGHT_is_pressed = false;
						break;
					default: {
						// NOTHING
					}
				}
				break;
			}
			case sf::Event::KeyPressed: {
				if (event.key.code != sf::Keyboard::Key::Unknown) {
					input_handler.key_is_pressed[event.key.code] = true;
				}
				break;
			}
			case sf::Event::KeyReleased: {
				if (event.key.code != sf::Keyboard::Key::Unknown) {
					input_handler.key_is_pressed[event.key.code] = false;
				}
				break;
			}
			default: {
				// NOTHING
			}
		}
	}
}

void GameEngine::handle_user_input() {
	if (input_handler.mouseLEFT_is_pressed) {
		const sf::Vector2i cur_position = sf::Mouse::getPosition(window);
		
		
		input_handler.mouseLEFT_last_press = cur_position;
	}

	input_handler.mouseLEFT_was_pressed = input_handler.mouseLEFT_is_pressed;
	input_handler.mouseRIGHT_was_pressed = input_handler.mouseRIGHT_is_pressed;


}

void GameEngine::show_debug_info() {
	const auto [avg_fps, min_fps] = refresh_rate.get_fps_info();

	fps_text.setString(std::to_string((int) std::round(avg_fps)) + " " + std::to_string((int) std::round(min_fps)));

	window.draw(fps_text);
}

GameEngine::~GameEngine() {
	delete[] screen_pixels;
}
