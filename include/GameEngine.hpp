#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

#include "FallingSandEngine.hpp"

constexpr float WANTED_FRAMES_PER_SECOND = 60.0f;
constexpr float WANTED_SECONDS_PER_FRAME = 1.0f / WANTED_FRAMES_PER_SECOND;

constexpr int FPS_BUFFER_SIZE = 120;

class GameEngine {
public:
	GameEngine(FallingSandEngine &fallingSandEngine_, const int window_width, const int window_height);

	GameEngine(FallingSandEngine &fallingSandEngine_);

	void setup();

	void run();

	void handle_user_input();

	void show_fps();

	float min_fps();

	float avg_fps();

	~GameEngine();

private:
	sf::RenderWindow window;
	FallingSandEngine &fallingSandEngine;

	sf::Texture screen_texture;
	sf::Sprite screen_sprite;
	sf::Uint8* screen_pixels;

	sf::Font font;

	float elapsed_time;
	float time_since_last_frame;
	std::chrono::system_clock::time_point last_frame_start;

	float last_fps[FPS_BUFFER_SIZE];
	int last_fps_idx = FPS_BUFFER_SIZE;

	sf::Event event;

};
