#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <SFML/System/Vector2.hpp>

class Chunk {
 public:
  Chunk() = default;

  explicit Chunk(const sf::Vector2i position) : position_(position) {}

  [[nodiscard]] bool IsActive() const { return activity_; }

  void SetActivity(const bool activity) { activity_ = activity; }

 private:
  sf::Vector2i position_;
  bool activity_;
};

#endif /* CHUNK_HPP_ */
