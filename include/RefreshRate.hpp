#ifndef REFRESH_RATE_HPP_
#define REFRESH_RATE_HPP_

#include <array>
#include <cstdint>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "CommonConstants.hpp"

class RefreshRate {
 public:
  using Frame_counter_t = uint64_t;

  RefreshRate();

  void Reset();

  void ResetFrameTime();

  [[nodiscard]] bool IsTimeForNewFrame() const;

  [[nodiscard]] double GetTotalElapsedTime() const;
  [[nodiscard]] double GetFrameElapsedTime() const;

  [[nodiscard]] std::pair<double, double> GetFpsInfo() const;

  [[nodiscard]] Frame_counter_t GetFrameCount() const { return frame_counter_; };

 private:
  Frame_counter_t frame_counter_{};

  sf::Clock start_clock_;
  sf::Clock last_frame_start_clock_;

  std::array<double, constants::kFpsBufferSize> past_fps_buffer_;
};

#endif /* REFRESH_RATE_HPP_ */
