#include "RefreshRate.hpp"

template <typename T>
RefreshRate<T>::RefreshRate() {
  reset();
}

template <typename T>
void RefreshRate<T>::reset() {
  start_clock = sf::Clock();
  last_frame_start_clock_ = sf::Clock();
  past_fps_buffer_ = std::deque<T>(FPS_BUFFER_SIZE, 0.0f);
}

template <typename T>
T RefreshRate<T>::get_elapsed_time() const {
  return start_clock.getElapsedTime().asSeconds();
}

template <typename T>
T RefreshRate<T>::get_time_since_last_frame() const {
  return last_frame_start_clock_.getElapsedTime().asSeconds();
}

template <typename T>
void RefreshRate<T>::reset_time_since_last_frame() {
  past_fps_buffer_.pop_front();
  past_fps_buffer_.push_back(1.0f / get_time_since_last_frame());

  last_frame_start_clock_.restart();
}

template <typename T>
std::pair<T, T> RefreshRate<T>::get_fps_info() const {
  T min_fps = __FLT_MAX__;
  T avg_fps = 0;

  for (const T x: past_fps_buffer_) {
    min_fps = std::min(min_fps, x);
    avg_fps += x;
  }
  
  avg_fps /= FPS_BUFFER_SIZE;

  return { avg_fps, min_fps };
}

template class RefreshRate<float>;
