#include "world/ChunkManager.hpp"

#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"

ChunkManager::ChunkManager(const sf::Vector2i size) : size_(size) {
  chunks_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (int i = 0; i < size_.y; ++i) {
    for (int j = 0; j < size_.x; ++j) {
      const int index = i * size_.x + j;
      chunks_[index] = Chunk({j, i});
    }
  }
}

bool ChunkManager::IsActive(const int pos_index) const {
  return chunks_[pos_index].IsActive();
}

bool ChunkManager::IsActive(const sf::Vector2i position) const {
  const int index = position.y * size_.x + position.x;
  return chunks_[index].IsActive();
}

void ChunkManager::SetActive(const sf::Vector2i position, const bool activity_) {
  const int index = position.y * size_.x + position.x;
  chunks_[index].SetActivity(activity_);
}
