#include "GameEngine.hpp"
#include "FallingSandEngine.hpp"

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

	std::fill(last_fps, last_fps + FPS_BUFFER_SIZE, 0.0f);

	screen_pixels = new sf::Uint8[window.getSize().x * window.getSize().y * 4];

	time_since_last_frame = 0.0f;
	last_frame_start = std::chrono::system_clock::now();
}

void GameEngine::run() {
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			#pragma GCC diagnostic ignored "-Wswitch"
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			}
		}
		
		const std::chrono::duration<float> elapsed_time_duration = std::chrono::system_clock::now() - last_frame_start;
		elapsed_time = elapsed_time_duration.count();
		time_since_last_frame += elapsed_time;
		last_frame_start = std::chrono::system_clock::now();

		handle_user_input();

		if (time_since_last_frame >= WANTED_SECONDS_PER_FRAME) {
			time_since_last_frame = 0.0f;
			last_fps[++last_fps_idx % FPS_BUFFER_SIZE] = 1.0f / elapsed_time;

			fallingSandEngine.draw_world_on_texture(screen_pixels);
			screen_texture.update(screen_pixels);

			window.draw(screen_sprite);
			window.display();

			fallingSandEngine.update();
		}
	}
}

void GameEngine::handle_user_input() {
	if (window.hasFocus()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			const sf::Vector2i cur_position = sf::Mouse::getPosition(window);
			
			fallingSandEngine.set_cell(cur_position.y, cur_position.x, Substance::SAND);
		}
	}
}

void GameEngine::show_fps() {
	sf::Text fps_text;
	fps_text.setFillColor(sf::Color::Green);
	fps_text.setFont(font);
	fps_text.setCharacterSize(10);

	fps_text.setString(std::to_string((avg_fps())) + " " + std::to_string((min_fps())));

	window.draw(fps_text);
}

float GameEngine::min_fps() {
	float min = 100000.0f;

	for (const float &x: last_fps)
		min = std::min(min, x);

	return min;
}

float GameEngine::avg_fps() {
	float sum = 0.0f;

	for (const float &x: last_fps)
		sum += x;

	return sum / FPS_BUFFER_SIZE;
}

GameEngine::~GameEngine() {
	delete[] screen_pixels;
}
