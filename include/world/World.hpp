#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "RandomNumberGenerators.hpp"
#include "world/ChunkManager.hpp"
#include "world/Element.hpp"

class World {
 public:
  World() = delete;

  explicit World(sf::Vector2i size);
  explicit World(sf::Vector2u size);

  [[nodiscard]] sf::Vector2i GetSize() const { return size_; }

  [[nodiscard]] Element& GetElementAt(size_t index);
  [[nodiscard]] Element& GetElementAt(sf::Vector2i pos);

  void SetElementAt(size_t index, Element element);
  void SetElementAt(sf::Vector2i pos, Element element);

  [[nodiscard]] sf::Color GetColorAt(sf::Vector2i pos) const;
  [[nodiscard]] sf::Color GetColorAt(size_t index) const;

  void SwapElements(size_t index1, size_t index2);

  void Update();

  void SetChunkActivity();

  [[nodiscard]] size_t GetElementCount() const { return static_cast<size_t>(size_.x) * size_.y; }

  [[nodiscard]] const void* GetElementsPointer() { return elements_.data(); }

  [[nodiscard]] bool IsChunkActive(sf::Vector2i position) const;

 private:
  sf::Vector2i size_;

  ChunkManager chunk_manager_;
  std::vector<Element> elements_;
  std::vector<bool> visited_;
  FastRng rng_;

  [[nodiscard]] sf::Color GetColorAtNoFail(size_t index) const;

  bool GovernLaw(sf::Vector2i position);

  template <engine::Substance substance>
  bool GovernLaw(Element& element, sf::Vector2i position);

  void UpdateChunkNeighborhood(int chunk_x, int chunk_y);

  [[nodiscard]] bool CanAccess(sf::Vector2i position);

  [[nodiscard]] bool CanAccessWithRandomVisit(sf::Vector2i position, engine::Substance original_subs);
};

#endif /* WORLD_HPP_ */
