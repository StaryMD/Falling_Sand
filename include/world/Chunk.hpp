#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

class Chunk {
 public:
  Chunk() = default;

  explicit Chunk(const sf::Vector2<int32_t> position) : position_(position) {}

  [[nodiscard]] bool IsActive(const int8_t step) const { return activity_[step]; }

  void SetActivity(const bool activity, const int8_t step) { activity_[step] = activity; }

 private:
  sf::Vector2<int32_t> position_;
  std::array<bool, 2> activity_;
};

#endif /* CHUNK_HPP_ */
