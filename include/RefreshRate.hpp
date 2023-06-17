#ifndef REFRESH_RATE_HPP_
#define REFRESH_RATE_HPP_

#include <deque>

#include <SFML/Graphics.hpp>

#include "CommonConstants.hpp"

constexpr float kWantedSecondsPerFrame = 1.0F / kWantedFramesPerSecond;

template <typename T>
class RefreshRate {
 public:
  RefreshRate();
  void Reset();

  [[nodiscard]] T GetElapsedTime() const;
  [[nodiscard]] T GetTimeSinceLastFrame() const;
  void ResetTimeSinceLastFrame();

  [[nodiscard]] std::pair<T, T> GetFpsInfo() const;

 private:
  sf::Clock start_clock_;
  sf::Clock last_frame_start_clock_;

  std::deque<T> past_fps_buffer_;
};

#endif /* REFRESH_RATE_HPP_ */
