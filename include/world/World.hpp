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

 private:
  sf::Vector2i size_;

  std::vector<Element> elements_;
};

#endif /* WORLD_HPP_ */
