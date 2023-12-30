#include "MasterEngine/RefreshRate.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>

#include <SFML/System/Clock.hpp>

#include "CommonConstants.hpp"

RefreshRate::RefreshRate() {
  Reset();
}

bool RefreshRate::IsTimeForNewFrame() const {
  return GetFrameElapsedTime() >= constants::kWantedSecondsPerFrame;
}

void RefreshRate::Reset() {
  frame_counter_ = 0;
  start_clock_ = sf::Clock();
  last_frame_start_clock_ = sf::Clock();
  past_fps_buffer_.fill(0.0);
}

double RefreshRate::GetTotalElapsedTime() const {
  return start_clock_.getElapsedTime().asSeconds();
}

double RefreshRate::GetFrameElapsedTime() const {
  return last_frame_start_clock_.getElapsedTime().asSeconds();
}

void RefreshRate::ResetFrameTime() {
  past_fps_buffer_[(frame_counter_++) % constants::kFpsBufferSize] = static_cast<double>(1.0) / GetFrameElapsedTime();

  last_frame_start_clock_.restart();
}

std::pair<double, double> RefreshRate::GetFpsInfo() const {
  double min_fps = __DBL_MAX__;
  double avg_fps = 0;

  for (const double fps_count : past_fps_buffer_) {
    min_fps = std::min(min_fps, fps_count);
    avg_fps += fps_count;
  }

  const size_t cat = std::min(frame_counter_, constants::kFpsBufferSize);
  avg_fps /= static_cast<double>(cat);

  return {avg_fps, min_fps};
}
