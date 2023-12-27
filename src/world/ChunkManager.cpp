#include "world/ChunkManager.hpp"

#include <cstddef>
#include <cstdint>

#include <SFML/System/Vector2.hpp>

#include "world/Chunk.hpp"

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
  return chunks_[pos_index].IsActive(current_step_);
}

bool ChunkManager::IsActive(const sf::Vector2i position) const {
  const int pos_index = position.y * size_.x + position.x;
  return chunks_[pos_index].IsActive(current_step_);
}

void ChunkManager::SetNextStepActivity(const sf::Vector2i position, const bool activity_) {
  const int pos_index = position.y * size_.x + position.x;
  chunks_[pos_index].SetActivity(activity_, static_cast<int8_t>(current_step_ == 0));
}

void ChunkManager::SwapBuffers() {
  for (Chunk& chunk : chunks_) {
    chunk.SetActivity(false, current_step_);
  }

  current_step_ = static_cast<int8_t>(current_step_ == 0);
}
