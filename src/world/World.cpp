#include "world/World.hpp"

#include <algorithm>
#include <cstddef>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "world/ChunkManager.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"

World::World(const sf::Vector2i size)
    : size_(size), chunk_manager_({constants::kChunkNumHorizontal, constants::kChunkNumVertical}) {
  elements_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (Element& element : elements_) {
    element = Element(engine::Substance::kAir);
  }

  visited_.resize(static_cast<size_t>(size_.x) * size_.y);
}

World::World(const sf::Vector2u size) : World(sf::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y))) {}

void World::Update() {
  using constants::kChunkSize;
  std::fill(visited_.begin(), visited_.end(), false);
  chunk_manager_.SwapBuffers();

  for (int chunk_y = chunk_manager_.GetSize().y - 1; chunk_y >= 0; --chunk_y) {
    for (int pos_y = kChunkSize - 1; pos_y >= 0; --pos_y) {
      for (int chunk_x = 0; chunk_x < chunk_manager_.GetSize().x; ++chunk_x) {
        if (chunk_manager_.IsActive({chunk_x, chunk_y})) {
          bool chunk_was_updated = false;

          if (rng_.NextRandValue() & 1) {
            for (int pos_x = 0; pos_x < kChunkSize; ++pos_x) {
              const sf::Vector2i element_position(chunk_x * kChunkSize + pos_x, chunk_y * kChunkSize + pos_y);
              const int index = element_position.y * constants::kWorldWidth + element_position.x;

              if (not visited_[index]) {
                chunk_was_updated |= GovernLaw(element_position);
              }
            }
          } else {
            for (int pos_x = kChunkSize - 1; pos_x >= 0; --pos_x) {
              const sf::Vector2i element_position(chunk_x * kChunkSize + pos_x, chunk_y * kChunkSize + pos_y);
              const int index = element_position.y * constants::kWorldWidth + element_position.x;

              if (not visited_[index]) {
                chunk_was_updated |= GovernLaw(element_position);
              }
            }
          }

          if (chunk_was_updated) {
            UpdateChunkNeighborhood(chunk_x, chunk_y);
          }
        }
      }
    }
  }
}

void World::UpdateChunkNeighborhood(const int chunk_x, const int chunk_y) {
  const int start_chunk_x = std::max(chunk_x - 1, 0);
  const int start_chunk_y = std::max(chunk_y - 1, 0);

  const int end_chunk_x = std::min(chunk_x + 1, chunk_manager_.GetSize().x - 1);
  const int end_chunk_y = std::min(chunk_y + 1, chunk_manager_.GetSize().y - 1);

  for (int dy = start_chunk_y; dy <= end_chunk_y; ++dy) {
    for (int dx = start_chunk_x; dx <= end_chunk_x; ++dx) {
      chunk_manager_.SetNextActive({dx, dy});
    }
  }
}

Element World::GetElementAt(const size_t index) const {
  return elements_[index];
}

Element World::GetElementAt(const sf::Vector2i pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetElementAt(index);
}

void World::SetElementAt(const size_t index, const Element element) {
  elements_[index] = element;
}

void World::SetElementAt(const sf::Vector2i pos, const Element element) {
  const size_t index = pos.y * size_.x + pos.x;
  SetElementAt(index, element);
  chunk_manager_.SetNextActive(pos / constants::kChunkSize);
}

sf::Color World::GetColorAt(const size_t index) const {
  return GetElementAt(index).GetColor();
}

sf::Color World::GetColorAt(const sf::Vector2i pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetColorAt(index);
}

void World::SwapElements(const size_t index1, const size_t index2) {
  std::swap(elements_[index1], elements_[index2]);
  visited_[index1] = visited_[index2] = true;
}

bool World::IsChunkActive(const sf::Vector2i position) const {
  return chunk_manager_.IsActive(position);
}

bool World::CanAccess(const sf::Vector2i position) {
  const bool is_inside =
      position.y < constants::kWorldHeight && position.y >= 0 && position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return false;
  }

  const int index = position.y * constants::kWorldWidth + position.x;

  return !visited_[index];
}

bool World::CanAccessWithRandomVisit(const sf::Vector2i position) {
  const bool is_inside =
      position.y < constants::kWorldHeight && position.y >= 0 && position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return false;
  }

  const int index = position.y * constants::kWorldWidth + position.x;
  const bool skip_visit_check = 0 == (rng_.NextRandValue() % constants::kChanceToIgnoreVisitedness);

  return !(skip_visit_check || visited_[index]);
}
