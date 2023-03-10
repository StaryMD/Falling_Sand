#include "GameEngine.hpp"
#include "FallingSandEngine.hpp"

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

	chosen_substance = Substance::SAND;
}

void GameEngine::run() {
	while (window.isOpen()) {
		handle_events();
		
		if (window.hasFocus()) {
			handle_user_input();
		}

		if (1 || refresh_rate.get_time_since_last_frame() >= WANTED_SECONDS_PER_FRAME) {
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

void GameEngine::handle_events() {
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
		}
	}
}

void GameEngine::handle_user_input() {
	if (input_handler.mouseLEFT_is_pressed) {
		const sf::Vector2i cur_position = sf::Mouse::getPosition(window);
		
		if (input_handler.mouseLEFT_was_pressed) {
			set_line_of_elements(cur_position, input_handler.mouseLEFT_last_press, Element(chosen_substance));
		}
		
		fallingSandEngine.set_cell(cur_position.y, cur_position.x, Element(chosen_substance), true);

		input_handler.mouseLEFT_last_press = cur_position;
	}

	input_handler.mouseLEFT_was_pressed = input_handler.mouseLEFT_is_pressed;
	input_handler.mouseRIGHT_was_pressed = input_handler.mouseRIGHT_is_pressed;


	if (input_handler.key_is_pressed[sf::Keyboard::Key::Num1]) {
		chosen_substance = Substance::AIR;
	} else if (input_handler.key_is_pressed[sf::Keyboard::Key::Num2]) {
		chosen_substance = Substance::SAND;
	} else if (input_handler.key_is_pressed[sf::Keyboard::Key::Num3]) {
		chosen_substance = Substance::STONE;
	} else if (input_handler.key_is_pressed[sf::Keyboard::Key::Num4]) {
		chosen_substance = Substance::WATER;
	}
}

void GameEngine::show_fps() {
	const auto [avg_fps, min_fps] = refresh_rate.get_fps_info();

	fps_text.setString(std::to_string((int) std::round(avg_fps)) + " " + std::to_string((int) std::round(min_fps)));

	window.draw(fps_text);
}

void GameEngine::set_line_of_elements(const sf::Vector2i &pos1, const sf::Vector2i &pos2, const Element element) {
	float dx = (pos2.x - pos1.x);
	float dy = (pos2.y - pos1.y);

	const int step = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);
	dx /= step;
	dy /= step;

	float x = pos1.x;
	float y = pos1.y;

	for (int i = 1; i <= step; i++) {
		fallingSandEngine.set_cell(y, x, element, true);
		x += dx;
		y += dy;
	}
}

GameEngine::~GameEngine() {
	delete[] screen_pixels;
}
