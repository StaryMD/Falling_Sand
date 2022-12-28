#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>

#include "world.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Falling Sand");
	const int windowWidth = window.getSize().x;
	const int windowHeight = window.getSize().y;
	float elapsed_time;

	World world;

	sf::Color color_of_substance[] = {
		sf::Color::Red,
		sf::Color(51, 51, 51),
		sf::Color::Yellow
	};

	sf::Texture screen_texture;
	screen_texture.create(windowWidth, windowHeight);
	sf::Sprite screen_sprite(screen_texture);
	sf::Uint8* screen_pixels = new sf::Uint8[windowWidth * windowHeight * 4];

	sf::Font font;
	font.loadFromFile("fonts/Lato-Regular.ttf");

	sf::Text fps_text;
	fps_text.setFillColor(sf::Color::Green);
	fps_text.setFont(font);
	fps_text.setCharacterSize(10);

	constexpr float wanted_frames_per_second = 60.0f;
	constexpr float wanted_time_per_frame = 1.0f / wanted_frames_per_second;
	float time_since_last_frame = 0.0f;

	constexpr int fps_buff_len = 120;
	float last_fps[fps_buff_len] = { 0.0f };
	int last_fps_idx = fps_buff_len;
	auto min_fps = [&last_fps]() {
		float min = 100000.0f;

		for (const float &x: last_fps)
			min = std::min(min, x);

		return min;
	};
	auto avg_fps = [&last_fps]() {
		float sum = 0.0f;

		for (float &x: last_fps)
			sum += x;

		return sum / fps_buff_len;
	};

	sf::Event event;
	std::chrono::system_clock::time_point last_frame_start = std::chrono::system_clock::now();
	while (window.isOpen()) {
		const std::chrono::duration<float> elapsed_time_duration = std::chrono::system_clock::now() - last_frame_start;
		elapsed_time = elapsed_time_duration.count();
		time_since_last_frame += elapsed_time;
		last_frame_start = std::chrono::system_clock::now();

		while (window.pollEvent(event)) {
#pragma GCC diagnostic ignored "-Wswitch"
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		if (window.hasFocus()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				const sf::Vector2i cur_position =  sf::Mouse::getPosition(window);
				world.set_subs_at(cur_position.y, cur_position.x, Substance::SAND);
			}
		}

		if (time_since_last_frame >= wanted_time_per_frame) {
			time_since_last_frame = 0.0f;
			last_fps[++last_fps_idx % fps_buff_len] = 1.0f / elapsed_time;

			{
				sf::Color* pixel_ptr = (sf::Color *) screen_pixels;

				for (int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH; i++) {
					*pixel_ptr = color_of_substance[(int) world.grid[i]];
					pixel_ptr++;
				}
				
				screen_texture.update(screen_pixels);
			}

			fps_text.setString(std::to_string((avg_fps())) + " " + std::to_string((min_fps())));

			//window.clear();

			window.draw(screen_sprite);

			window.draw(fps_text);

			window.display();

			world.update();
		}
	}

	delete[] screen_pixels;
	return 0;
}
