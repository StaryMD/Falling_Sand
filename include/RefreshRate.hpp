#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <deque>
#include <utility>

constexpr float WANTED_FRAMES_PER_SECOND = 60.0f;
constexpr float WANTED_SECONDS_PER_FRAME = 1.0f / WANTED_FRAMES_PER_SECOND;

constexpr int FPS_BUFFER_SIZE = 120;

class RefreshRate {
public:
	void setup();

	float get_elapsed_time();
	float get_time_since_last_frame();
	void reset_time_since_last_frame();

	std::pair<float, float> get_fps_info();

private:
	sf::Clock start_clock;
	sf::Clock last_frame_start_clock;

	std::deque<float> past_fps_buffer;

};
