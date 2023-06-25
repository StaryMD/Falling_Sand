#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "world/Element.hpp"

class World {
 public:
  World() = delete;

  explicit World(sf::Vector2i size);
  explicit World(sf::Vector2u size);

  [[nodiscard]] sf::Vector2i GetSize() const { return size_; }

  [[nodiscard]] Element GetElementAt(size_t index) const;
  [[nodiscard]] Element GetElementAt(sf::Vector2i pos) const;

  void SetElementAt(size_t index, Element element);
  void SetElementAt(sf::Vector2i pos, Element element);

  [[nodiscard]] sf::Color GetColorAt(sf::Vector2i pos) const;
  [[nodiscard]] sf::Color GetColorAt(size_t index) const;

  void Update();

  [[nodiscard]] size_t GetElementCount() const { return static_cast<size_t>(size_.x) * size_.y; }

  [[nodiscard]] const void* GetElementsPointer() { return elements_.data(); }

 private:
  sf::Vector2i size_;

  std::vector<Element> elements_;

  [[nodiscard]] sf::Color GetColorAtNoFail(size_t index) const;

  void GovernLaw(sf::Vector2i position);

  template <engine::Substance substance>
  void GovernLaw(Element element, sf::Vector2i position);
};

#endif /* WORLD_HPP_ */
