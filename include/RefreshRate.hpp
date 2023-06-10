#ifndef REFRESH_RATE_HPP_
#define REFRESH_RATE_HPP_

#include <deque>

#include <SFML/Graphics.hpp>

#include "common_constants.hpp"

constexpr float WANTED_SECONDS_PER_FRAME = 1.0f / WANTED_FRAMES_PER_SECOND;

template <typename T>
class RefreshRate {
 public:
  RefreshRate();
  void reset();

  T get_elapsed_time() const;
  T get_time_since_last_frame() const;
  void reset_time_since_last_frame();

  std::pair<T, T> get_fps_info() const;

 private:
  sf::Clock start_clock;
  sf::Clock last_frame_start_clock_;

  std::deque<T> past_fps_buffer_;
};

#endif /* REFRESH_RATE_HPP_ */
