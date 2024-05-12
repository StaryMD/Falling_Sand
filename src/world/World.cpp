#include "World/World.hpp"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "CommonUtility.hpp"
#include "RandomNumberGenerators.hpp"
#include "World/ChunkManager.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"

World::World(const sf::Vector2<uint32_t> size)
    : size_(ToVector2<int32_t>(size)), chunk_manager_({constants::kChunkNumHorizontal, constants::kChunkNumVertical}) {
  elements_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (Element& element : elements_) {
    element = Element(engine::Substance::kAir);
  }

  visited_.resize(static_cast<size_t>(size_.x) * size_.y);
  // update_threads_ = static_cast<int32_t>(std::thread::hardware_concurrency()) - 1;
  update_threads_ = 0;
}

void World::Update() {
  using constants::kChunkSize;
  std::fill(visited_.begin(), visited_.end(), false);
  chunk_manager_.SwapBuffers();
  rng_ = FastRng();

  chunks_updated_count_ = 0;

  if (update_threads_ != 0) {
    std::atomic<uint32_t> local_chunks_updated_count = 0;

    auto update_odd_chunks = [this, &local_chunks_updated_count](const int32_t thread_id) {
      const int32_t total_chunk_count = chunk_manager_.GetSize().y * chunk_manager_.GetSize().x;
      const int32_t chunks_to_update_count = (total_chunk_count + update_threads_ - 1) / update_threads_;

      uint32_t chunks_updated = 0;

      for (int32_t i = 0; i < chunks_to_update_count; ++i) {
        const int32_t index = i * update_threads_ + thread_id;

        const int32_t chunk_y = index / chunk_manager_.GetSize().x;
        const int32_t chunk_x = index % chunk_manager_.GetSize().x;

        const bool chunk_is_valid = (chunk_y < chunk_manager_.GetSize().y) && (chunk_x < chunk_manager_.GetSize().x);

        if (chunk_is_valid && chunk_manager_.IsActive({chunk_x, chunk_y})) {
          ++chunks_updated;
          bool chunk_was_updated = false;

          int32_t pos_y = 0;
          int32_t step_y = 1;

          if (rng_.NextValue() & 1) {
            pos_y = kChunkSize - 1;
            step_y = -1;
          }

          for (int32_t _ = 0; _ != kChunkSize; ++_, pos_y += step_y) {
            int32_t pos_x = 0;
            int32_t step_x = 1;

            if (rng_.NextValue() & 1) {
              pos_x = kChunkSize - 1;
              step_x = -1;
            }

            for (int32_t _ = 0; _ != kChunkSize; ++_, pos_x += step_x) {
              const sf::Vector2<int32_t> element_position(chunk_x * kChunkSize + pos_x, chunk_y * kChunkSize + pos_y);
              const int32_t index = element_position.y * constants::kWorldWidth + element_position.x;

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

      local_chunks_updated_count += chunks_updated;
    };

    std::vector<std::thread> worker_threads;
    worker_threads.reserve(update_threads_ - 1);

    for (int32_t i = 1; i < update_threads_; ++i) {
      worker_threads.emplace_back(update_odd_chunks, i);
    }

    update_odd_chunks(0);

    for (auto& thr : worker_threads) {
      thr.join();
    }

    chunks_updated_count_ = local_chunks_updated_count;
  } else {
    for (int32_t chunk_y = 0; chunk_y < chunk_manager_.GetSize().y; ++chunk_y) {
      for (int32_t chunk_x = 0; chunk_x < chunk_manager_.GetSize().x; ++chunk_x) {
        if (chunk_manager_.IsActive({chunk_x, chunk_y})) {
          ++chunks_updated_count_;
          bool chunk_was_updated = false;

          int32_t pos_y = 0;
          int32_t step_y = 1;

          if (rng_.NextValue() & 1) {
            pos_y = kChunkSize - 1;
            step_y = -1;
          }

          for (int32_t _ = 0; _ != kChunkSize; ++_, pos_y += step_y) {
            int32_t pos_x = 0;
            int32_t step_x = 1;

            if (rng_.NextValue() & 1) {
              pos_x = kChunkSize - 1;
              step_x = -1;
            }

            for (int32_t _ = 0; _ != kChunkSize; ++_, pos_x += step_x) {
              const sf::Vector2<int32_t> element_position(chunk_x * kChunkSize + pos_x, chunk_y * kChunkSize + pos_y);
              const int32_t index = element_position.y * constants::kWorldWidth + element_position.x;

              if (not visited_[index]) {
                const bool something_changed = GovernLaw(element_position);

                if (something_changed) {
                  chunk_was_updated = true;
                }
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

void World::UpdateChunkNeighborhood(const int32_t chunk_x, const int32_t chunk_y) {
  const int32_t start_chunk_x = std::max(chunk_x - 1, 0);
  const int32_t start_chunk_y = std::max(chunk_y - 1, 0);

  const int32_t end_chunk_x = std::min(chunk_x + 1, chunk_manager_.GetSize().x - 1);
  const int32_t end_chunk_y = std::min(chunk_y + 1, chunk_manager_.GetSize().y - 1);

  for (int32_t dy = start_chunk_y; dy <= end_chunk_y; ++dy) {
    for (int32_t dx = start_chunk_x; dx <= end_chunk_x; ++dx) {
      chunk_manager_.SetNextStepActivity({dx, dy});
    }
  }
}

Element& World::GetElementAt(const sf::Vector2<int32_t> pos) {
  const size_t index = pos.y * size_.x + pos.x;
  return elements_[index];
}

engine::Substance World::GetSubstanceAt(const sf::Vector2<int32_t> pos) const {
  return GetElementAt(pos).GetSubstance();
}

const Element& World::GetElementAt(const sf::Vector2<int32_t> pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return elements_[index];
}

void World::SetElementAt(const sf::Vector2<int32_t> position, const Element element) {
  const bool is_inside =
      position.y < constants::kWorldHeight && position.y >= 0 && position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return;
  }

  GetElementAt(position) = element;
  chunk_manager_.SetNextStepActivity(position / constants::kChunkSize);
}

void World::SwapElements(const sf::Vector2<int32_t> position1, const sf::Vector2<int32_t> position2) {
  std::swap(GetElementAt(position1), GetElementAt(position2));
  SetVisit(position1, true);
  SetVisit(position2, true);
}

bool World::IsChunkActive(const sf::Vector2<int32_t> position) const {
  return chunk_manager_.IsActive(position);
}

bool World::CanAccess(const sf::Vector2<int32_t> position) const {
  const bool is_inside =
      position.y < constants::kWorldHeight && position.y >= 0 && position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return false;
  }

  return engine::IsMovable(GetSubstanceAt(position)) && !GetVisit(position);
}

bool World::CanAccessWithRandomVisit(const sf::Vector2<int32_t> position, const engine::Substance original_subs) {
  const int8_t chance_to_ignore_visitedness = engine::GetChanceToIgnoreVisitedness(original_subs);

  const bool skip_visit_check =
      (chance_to_ignore_visitedness != -1) && (0 == (fastest_rng_.NextValue() % chance_to_ignore_visitedness));

  return CanAccess(position) && !skip_visit_check;
}

uint8_t World::AirNeighbourCount(const sf::Vector2<int32_t> position) const {
  int8_t ans = 0;

  //NOLINTBEGIN(readability-implicit-bool-conversion,bugprone-narrowing-conversions)
  ans += (position.x - 1 >= 0) && (GetSubstanceAt(LeftBy(position)) == engine::Substance::kAir);
  ans += (position.x + 1 < constants::kWorldWidth) && (GetSubstanceAt(RightBy(position)) == engine::Substance::kAir);
  ans += (position.y - 1 >= 0) && (GetSubstanceAt(DownBy(position)) == engine::Substance::kAir);
  ans += (position.y + 1 < constants::kWorldHeight) && (GetSubstanceAt(UpBy(position)) == engine::Substance::kAir);
  //NOLINTEND(readability-implicit-bool-conversion,bugprone-narrowing-conversions)
  return ans;
}

bool World::GovernLaw(const sf::Vector2<int32_t> position) {
  Element& element = GetElementAt(position);

  switch (element.GetSubstance()) {
    case engine::Substance::kAir: {
      return GovernLaw<engine::Substance::kAir>(element, position);
    }
    case engine::Substance::kSand: {
      return GovernLaw<engine::Substance::kSand>(element, position);
    }
    case engine::Substance::kStone: {
      return GovernLaw<engine::Substance::kStone>(element, position);
    }
    case engine::Substance::kWater: {
      return GovernLaw<engine::Substance::kWater>(element, position);
    }
    case engine::Substance::kOil: {
      return GovernLaw<engine::Substance::kOil>(element, position);
    }
    case engine::Substance::kSteam: {
      return GovernLaw<engine::Substance::kSteam>(element, position);
    }
    case engine::Substance::kFire: {
      return GovernLaw<engine::Substance::kFire>(element, position);
    }
    case engine::Substance::kSmoke: {
      return GovernLaw<engine::Substance::kSmoke>(element, position);
    }
    default: {
      // unreachable (hopefully)
    }
  }

  return false;
}

sf::Vector2<int32_t> World::GetSize() const {
  return size_;
}

size_t World::GetElementCount() const {
  return static_cast<size_t>(size_.x) * size_.y;
}

void* World::GetElementsPointer() {
  return elements_.data();
}

uint32_t World::GetChunksUpdatedCount() const {
  return chunks_updated_count_;
}

bool World::GetVisit(const sf::Vector2<int32_t> position) const {
  const size_t index = position.y * size_.x + position.x;
  return visited_[index];
}

void World::SetVisit(const sf::Vector2<int32_t> position, const bool visit) {
  const size_t index = position.y * size_.x + position.x;
  visited_[index] = visit;
}
