#include "RefreshRate.hpp"

#include "algorithm"

void RefreshRate::setup() {
	start_clock = sf::Clock();
	last_frame_start_clock = sf::Clock();
	past_fps_buffer = std::deque<float>(FPS_BUFFER_SIZE, 0.0f);
}

float RefreshRate::get_elapsed_time() {
	return start_clock.getElapsedTime().asSeconds();
}

float RefreshRate::get_time_since_last_frame() {
	return last_frame_start_clock.getElapsedTime().asSeconds();
}

void RefreshRate::reset_time_since_last_frame() {
	past_fps_buffer.pop_front();
	past_fps_buffer.push_back(1.0f / get_time_since_last_frame());

	last_frame_start_clock.restart();
}

std::pair<float, float> RefreshRate::get_fps_info() {
	float min_fps = __FLT_MAX__;
	float avg_fps = 0.0f;

	for (const float x: past_fps_buffer) {
		min_fps = std::min(min_fps, x);
		avg_fps += x;
	}
	
	avg_fps /= FPS_BUFFER_SIZE;

	return { avg_fps, min_fps };
}
