#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <cstddef>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "RandomNumberGenerators.hpp"
#include "World/ChunkManager.hpp"
#include "World/Element.hpp"

class World {
 public:
  World() = delete;

  explicit World(sf::Vector2<uint32_t> size);

  [[nodiscard]] sf::Vector2<int32_t> GetSize() const { return size_; }

  [[nodiscard]] Element& GetElementAt(size_t index);
  [[nodiscard]] Element& GetElementAt(sf::Vector2<int32_t> pos);

  [[nodiscard]] const Element& GetElementAt(size_t index) const;
  [[nodiscard]] const Element& GetElementAt(sf::Vector2<int32_t> pos) const;

  [[nodiscard]] engine::Substance GetSubstanceAt(sf::Vector2<int32_t> pos) const;

  void SetElementAt(size_t index, Element element);
  void SetElementAt(sf::Vector2<int32_t> pos, Element element);

  void SwapElements(size_t index1, size_t index2);

  void Update();

  void SetChunkActivity();

  [[nodiscard]] size_t GetElementCount() const { return static_cast<size_t>(size_.x) * size_.y; }

  [[nodiscard]] void* GetElementsPointer() { return elements_.data(); }

  [[nodiscard]] bool IsChunkActive(sf::Vector2<int32_t> position) const;

  [[nodiscard]] uint32_t GetChunksUpdatedCount() const { return chunks_updated_count_; }

  //NOLINTNEXTLINE
  int32_t update_threads_;

 private:
  sf::Vector2<int32_t> size_;

  ChunkManager chunk_manager_;
  std::vector<Element> elements_;
  std::vector<bool> visited_;
  FastRng rng_;
  Counter<uint32_t> fastest_rng_;

  uint32_t chunks_updated_count_;

  bool GovernLaw(sf::Vector2<int32_t> position);

  template <engine::Substance substance>
  bool GovernLaw(Element& element, sf::Vector2<int32_t> position);

  void UpdateChunkNeighborhood(int32_t chunk_x, int32_t chunk_y);

  [[nodiscard]] bool CanAccess(sf::Vector2<int32_t> position);

  [[nodiscard]] bool CanAccessWithRandomVisit(sf::Vector2<int32_t> position, engine::Substance original_subs);

  [[nodiscard]] uint8_t AirNeighbourCount(int32_t index) const;
};

#endif // WORLD_HPP_
