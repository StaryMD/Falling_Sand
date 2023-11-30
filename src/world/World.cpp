#include "world/World.hpp"

#include <algorithm>
#include <atomic>
#include <cstddef>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <thread>

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
  update_threads_ = static_cast<int>(std::thread::hardware_concurrency());
}

World::World(const sf::Vector2u size) : World(sf::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y))) {}

void World::Update() {
  using constants::kChunkSize;
  std::fill(visited_.begin(), visited_.end(), false);
  chunk_manager_.SwapBuffers();

  chunks_updated_count_ = 0;

  if (update_threads_ != 0) {
    std::atomic<unsigned> local_chunks_updated_count = 0;

    auto update_odd_chunks = [this, &local_chunks_updated_count](const int thread_id) {
      const int total_chunk_count = chunk_manager_.GetSize().y * chunk_manager_.GetSize().x;
      const int chunks_to_update_count = (total_chunk_count + update_threads_ - 1) / update_threads_;

      unsigned chunks_updated = 0;

      for (int i = 0; i < chunks_to_update_count; ++i) {
        const int index = i * update_threads_ + thread_id;

        const int chunk_y = index / chunk_manager_.GetSize().x;
        const int chunk_x = index % chunk_manager_.GetSize().x;

        const bool chunk_is_valid = (chunk_y < chunk_manager_.GetSize().y) && (chunk_x < chunk_manager_.GetSize().x);

        if (chunk_is_valid && chunk_manager_.IsActive({chunk_x, chunk_y})) {
          ++chunks_updated;
          bool chunk_was_updated = false;

          for (int pos_y = kChunkSize - 1; pos_y >= 0; --pos_y) {
            int pos_x = 0;
            int step_x = 1;

            if (rng_.NextRandValue() & 1) {
              pos_x = kChunkSize - 1;
              step_x = -1;
            }

            for (int _ = 0; _ != kChunkSize; ++_, pos_x += step_x) {
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

      local_chunks_updated_count += chunks_updated;
    };

    std::vector<std::thread> worker_threads;
    worker_threads.reserve(update_threads_ - 1);

    for (int i = 1; i < update_threads_; ++i) {
      worker_threads.emplace_back(update_odd_chunks, i);
    }

    update_odd_chunks(0);

    for (auto& thr : worker_threads) {
      thr.join();
    }

    chunks_updated_count_ = local_chunks_updated_count;
  } else {
    for (int chunk_y = 0; chunk_y < chunk_manager_.GetSize().y; ++chunk_y) {
      for (int chunk_x = 0; chunk_x < chunk_manager_.GetSize().x; ++chunk_x) {
        if (chunk_manager_.IsActive({chunk_x, chunk_y})) {
          ++chunks_updated_count_;
          bool chunk_was_updated = false;

          for (int pos_y = kChunkSize - 1; pos_y >= 0; --pos_y) {
            int pos_x = 0;
            int step_x = 1;

            if (rng_.NextRandValue() & 1) {
              pos_x = kChunkSize - 1;
              step_x = -1;
            }

            for (int _ = 0; _ != kChunkSize; ++_, pos_x += step_x) {
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

Element& World::GetElementAt(const size_t index) {
  return elements_[index];
}

Element& World::GetElementAt(const sf::Vector2i pos) {
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

bool World::CanAccessWithRandomVisit(const sf::Vector2i position, const engine::Substance original_subs) {
  const bool is_inside =
      position.y < constants::kWorldHeight && position.y >= 0 && position.x >= 0 && position.x < constants::kWorldWidth;

  if (not is_inside) {
    return false;
  }

  const int index = position.y * constants::kWorldWidth + position.x;

  const int8_t chance_to_ignore_visitedness = engine::GetChanceToIgnoreVisitedness(original_subs);

  const bool skip_visit_check =
      (chance_to_ignore_visitedness != -1) && (0 == (rng_.NextRandValue() % chance_to_ignore_visitedness));

  return !(skip_visit_check || visited_[index]);
}
