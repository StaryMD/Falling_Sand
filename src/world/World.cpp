#include "World/World.hpp"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
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
    : size_(ToVector2<int32_t>(size)),
      chunk_manager_({constants::kChunkNumHorizontal, constants::kChunkNumVertical}) {
  elements_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (Element& element : elements_) {
    element = Element(engine::Substance::kAir);
  }

  visited_.resize(static_cast<size_t>(size_.x) * size_.y);
  update_threads_ = static_cast<int32_t>(std::thread::hardware_concurrency()) - 1;
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
      const int32_t chunks_to_update_count =
          (total_chunk_count + update_threads_ - 1) / update_threads_;

      uint32_t chunks_updated = 0;

      for (int32_t i = 0; i < chunks_to_update_count; ++i) {
        const int32_t index = i * update_threads_ + thread_id;

        const int32_t chunk_y = index / chunk_manager_.GetSize().x;
        const int32_t chunk_x = index % chunk_manager_.GetSize().x;

        const bool chunk_is_valid =
            (chunk_y < chunk_manager_.GetSize().y) && (chunk_x < chunk_manager_.GetSize().x);

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
              const sf::Vector2<int32_t> element_position(chunk_x * kChunkSize + pos_x,
                                                          chunk_y * kChunkSize + pos_y);
              const int32_t index =
                  element_position.y * constants::kWorldWidth + element_position.x;

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
              const sf::Vector2<int32_t> element_position(chunk_x * kChunkSize + pos_x,
                                                          chunk_y * kChunkSize + pos_y);
              const int32_t index =
                  element_position.y * constants::kWorldWidth + element_position.x;

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

Element& World::GetElementAt(const size_t index) {
  return elements_[index];
}

Element& World::GetElementAt(const sf::Vector2<int32_t> pos) {
  const size_t index = pos.y * size_.x + pos.x;
  return GetElementAt(index);
}

engine::Substance World::GetSubstanceAt(const sf::Vector2<int32_t> pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetElementAt(index).GetSubstance();
}

const Element& World::GetElementAt(const size_t index) const {
  return elements_[index];
}

const Element& World::GetElementAt(const sf::Vector2<int32_t> pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetElementAt(index);
}

void World::SetElementAt(const size_t index, const Element element) {
  elements_[index] = element;
}

void World::SetElementAt(const sf::Vector2<int32_t> pos, const Element element) {
  const size_t index = pos.y * size_.x + pos.x;

  const bool is_inside =
      pos.y < constants::kWorldHeight && pos.y >= 0 && pos.x >= 0 && pos.x < constants::kWorldWidth;

  if (not is_inside) {
    return;
  }

  SetElementAt(index, element);
  chunk_manager_.SetNextStepActivity(pos / constants::kChunkSize);
}

void World::SwapElements(const size_t index1, const size_t index2) {
  std::swap(elements_[index1], elements_[index2]);
  visited_[index1] = visited_[index2] = true;
}

bool World::IsChunkActive(const sf::Vector2<int32_t> position) const {
  return chunk_manager_.IsActive(position);
}

bool World::CanAccess(const sf::Vector2<int32_t> position) {
  const bool is_inside = position.y < constants::kWorldHeight && position.y >= 0 &&
                         position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return false;
  }

  const int32_t index = position.y * constants::kWorldWidth + position.x;

  return engine::IsMovable(GetElementAt(index).GetSubstance()) && !visited_[index];
}

bool World::CanAccessWithRandomVisit(const sf::Vector2<int32_t> position,
                                     const engine::Substance original_subs) {
  const int8_t chance_to_ignore_visitedness = engine::GetChanceToIgnoreVisitedness(original_subs);

  const bool skip_visit_check = (chance_to_ignore_visitedness != -1) &&
                                (0 == (fastest_rng_.NextValue() % chance_to_ignore_visitedness));

  return CanAccess(position) && !skip_visit_check;
}

uint8_t World::AirNeighbourCount(const int32_t index) const {
  const int32_t pos_x = index % constants::kWorldWidth;
  const int32_t pos_y = index / constants::kWorldWidth;

  int32_t ans = 0;

  //NOLINTBEGIN(readability-implicit-bool-conversion)
  ans += (pos_x - 1 >= 0) && (GetElementAt(index - 1).GetSubstance() == engine::Substance::kAir);
  ans += (pos_x + 1 < constants::kWorldWidth) &&
         (GetElementAt(index + 1).GetSubstance() == engine::Substance::kAir);
  ans += (pos_y - 1 >= 0) &&
         (GetElementAt(index - constants::kWorldWidth).GetSubstance() == engine::Substance::kAir);
  ans += (pos_y + 1 < constants::kWorldHeight) &&
         (GetElementAt(index + constants::kWorldWidth).GetSubstance() == engine::Substance::kAir);
  //NOLINTEND(readability-implicit-bool-conversion)
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
