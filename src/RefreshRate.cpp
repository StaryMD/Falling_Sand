#include "RefreshRate.hpp"
#include "CommonConstants.hpp"

template <typename T>
RefreshRate<T>::RefreshRate() {
  Reset();
}

template <typename T>
void RefreshRate<T>::Reset() {
  start_clock_ = sf::Clock();
  last_frame_start_clock_ = sf::Clock();
  past_fps_buffer_ = std::deque<T>(kFpsBufferSize, 0.0F);
}

template <typename T>
T RefreshRate<T>::GetElapsedTime() const {
  return start_clock_.getElapsedTime().asSeconds();
}

template <typename T>
T RefreshRate<T>::GetTimeSinceLastFrame() const {
  return last_frame_start_clock_.getElapsedTime().asSeconds();
}

template <typename T>
void RefreshRate<T>::ResetTimeSinceLastFrame() {
  past_fps_buffer_.pop_front();
  past_fps_buffer_.push_back(1.0F / GetTimeSinceLastFrame());

  last_frame_start_clock_.restart();
}

template <typename T>
std::pair<T, T> RefreshRate<T>::GetFpsInfo() const {
  T min_fps = __FLT_MAX__;
  T avg_fps = 0;

  for (const T fps_count : past_fps_buffer_) {
    min_fps = std::min(min_fps, fps_count);
    avg_fps += fps_count;
  }

  avg_fps /= kFpsBufferSize;

  return {avg_fps, min_fps};
}

template class RefreshRate<float>;
