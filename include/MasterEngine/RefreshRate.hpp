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

  /**
   * @brief Start a interval timer at the current time.
   * 
   */
  RefreshRate();

  /**
   * @brief Reset the timer to start at the current time.
   * 
   */
  void Reset();

  /**
   * @brief Reset the time of the last frame recorded.
   * 
   */
  void ResetFrameTime();

  /**
   * @brief Calculate whether the time for the current frame is done and another needs to be started.
   * 
   * @return bool Whether it's to start a new frame.
   */
  [[nodiscard]] bool IsTimeForNewFrame() const;

  /**
   * @brief Get the total elapsed time since starting the timer.
   * 
   * @return double Total elapsed time.
   */
  [[nodiscard]] double GetTotalElapsedTime() const;

  /**
   * @brief Get the elapsed time inside the current frame.
   * 
   * @return double Elapsed time inside the current frame.
   */
  [[nodiscard]] double GetFrameElapsedTime() const;

  /**
   * @brief Get information regarding past frames.
   * 
   * @return std::pair<double, double> Pair consisting of the average FPS and minimum FPS over several past frames.
   */
  [[nodiscard]] std::pair<double, double> GetFpsInfo() const;

  /**
   * @brief Get the total frame counter since starting the timer.
   * 
   * @return Frame_counter_t Total frame counter.
   */
  [[nodiscard]] Frame_counter_t GetFrameCount() const { return frame_counter_; };

 private:
  Frame_counter_t frame_counter_{};

  sf::Clock start_clock_;
  sf::Clock last_frame_start_clock_;

  std::array<double, constants::kFpsBufferSize> past_fps_buffer_;
};

#endif  // REFRESH_RATE_HPP_
