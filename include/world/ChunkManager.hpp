#ifndef CHUNK_MANAGER_HPP_
#define CHUNK_MANAGER_HPP_

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "world/Chunk.hpp"

class ChunkManager {
 public:
  ChunkManager() = delete;

  explicit ChunkManager(sf::Vector2i size);

  [[nodiscard]] bool IsActive(int pos_index) const;

  [[nodiscard]] bool IsActive(sf::Vector2i position) const;

  [[nodiscard]] sf::Vector2i GetSize() const { return size_; };

  void SetNextStepActivity(sf::Vector2i position, bool activity_ = true);

  void SwapBuffers();

 private:
  sf::Vector2i size_;

  std::vector<Chunk> chunks_;
  int8_t current_step_{};
};

#endif /* CHUNK_MANAGER_HPP_ */
