#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>

#include "world.hpp"

constexpr float WANTED_FRAMES_PER_SECOND = 60.0f;
constexpr float WANTED_SECONDS_PER_FRAME = 1.0f / WANTED_FRAMES_PER_SECOND;

constexpr int FPS_BUFFER_SIZE = 120;

class GameEngine {
public:
	sf::RenderWindow window;

	GameEngine() {
		window.create(sf::VideoMode::getDesktopMode(), "Falling Sand", sf::Style::Fullscreen);
	}

	GameEngine(const int windows_height, const int windows_width) {
		window.create(sf::VideoMode(windows_width, windows_height), "Falling Sand");
	}

	void setup() {
		windowWidth = WORLD_WIDTH;
		windowHeight = WORLD_HEIGHT;

		screen_texture.create(windowWidth, windowHeight);
		screen_sprite.setTexture(screen_texture);

		font.loadFromFile("fonts/Lato-Regular.ttf");

		time_since_last_frame = 0.0f;
		last_frame_start = std::chrono::system_clock::now();
		
		std::fill(last_fps, last_fps + FPS_BUFFER_SIZE, 0.0f);

		screen_pixels = new sf::Uint8[windowWidth * windowHeight * 4];
	}

	void update() {
		const std::chrono::duration<float> elapsed_time_duration = std::chrono::system_clock::now() - last_frame_start;
		elapsed_time = elapsed_time_duration.count();
		time_since_last_frame += elapsed_time;
		last_frame_start = std::chrono::system_clock::now();

		handle_user_input();

		if (time_since_last_frame >= WANTED_SECONDS_PER_FRAME) {
			time_since_last_frame = 0.0f;
			last_fps[++last_fps_idx % FPS_BUFFER_SIZE] = 1.0f / elapsed_time;

			update_window();
		}
	}

	void update_window() {
		sf::Color* pixel_ptr = (sf::Color *) screen_pixels;

		for (int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH; i++) {
			*pixel_ptr = color_of_substance[(int) world.grid[i]];
			pixel_ptr++;
		}

		screen_texture.update(screen_pixels);

		window.draw(screen_sprite);

		window.display();

		world.update();
	}

	void handle_user_input() {
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
	}

	void show_fps() {
		sf::Text fps_text;
		fps_text.setFillColor(sf::Color::Green);
		fps_text.setFont(font);
		fps_text.setCharacterSize(10);

		fps_text.setString(std::to_string((avg_fps())) + " " + std::to_string((min_fps())));

		window.draw(fps_text);
	}

	~GameEngine() {
		delete[] screen_pixels;
	}

private:
	int windowWidth;
	int windowHeight;

	World world;

	sf::Color color_of_substance[SUBSTANCE_COUNT] = {
		sf::Color::Red,
		sf::Color(101, 101, 101),
		sf::Color::Yellow
	};

	sf::Texture screen_texture;
	sf::Sprite screen_sprite;
	sf::Uint8* screen_pixels;

	sf::Font font;

	float elapsed_time;
	float time_since_last_frame;

	float last_fps[FPS_BUFFER_SIZE];
	int last_fps_idx = FPS_BUFFER_SIZE;

	sf::Event event;
	std::chrono::system_clock::time_point last_frame_start;

	float min_fps() {
		float min = 100000.0f;

		for (const float &x: last_fps)
			min = std::min(min, x);

		return min;
	}

	float avg_fps() {
		float sum = 0.0f;

		for (const float &x: last_fps)
			sum += x;

		return sum / FPS_BUFFER_SIZE;
	}

};
