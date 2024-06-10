#ifndef CHUNK_MANAGER_HPP_
#define CHUNK_MANAGER_HPP_

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "World/Chunk.hpp"

class ChunkManager {
 public:
  ChunkManager() = delete;

  explicit ChunkManager(sf::Vector2<int32_t> size);

  [[nodiscard]] sf::Vector2<int32_t> GetSize() const { return size_; };

  [[nodiscard]] bool IsActive(int32_t pos_index) const;

  [[nodiscard]] bool IsActive(sf::Vector2<int32_t> position) const;

  void SetNextStepActivity(sf::Vector2<int32_t> position, bool activity_ = true);

  void SwapBuffers();

 private:
  sf::Vector2<int32_t> size_;

  std::vector<Chunk> chunks_;
  int8_t current_step_{};
};

#endif // CHUNK_MANAGER_HPP_
