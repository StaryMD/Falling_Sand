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

  void SetActive(sf::Vector2i position);

 private:
  sf::Vector2i size_;

  std::vector<Chunk> chunks_;
};

#endif /* CHUNK_MANAGER_HPP_ */
