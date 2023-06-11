#ifndef REFRESH_RATE_HPP_
#define REFRESH_RATE_HPP_

#include <deque>

#include <SFML/Graphics.hpp>

#include "common_constants.hpp"

constexpr float kWantedSecondsPerFrame = 1.0F / kWantedFramesPerSecond;

template <typename T>
class RefreshRate {
 public:
  RefreshRate();
  void Reset();

  T GetElapsedTime() const;
  T GetTimeSinceLastFrame() const;
  void ResetTimeSinceLastFrame();

  std::pair<T, T> GetFpsInfo() const;

 private:
  sf::Clock start_clock_;
  sf::Clock last_frame_start_clock_;

  std::deque<T> past_fps_buffer_;
};

#endif /* REFRESH_RATE_HPP_ */
