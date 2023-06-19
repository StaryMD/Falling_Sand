#include "world/World.hpp"

#include <SFML/Graphics/Color.hpp>
#include <cstdlib>

#include <SFML/System/Vector2.hpp>

#include "world/Element.hpp"
#include "world/Substance.hpp"

World::World(const sf::Vector2i size) : size_(size) {
  elements_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (Element& element : elements_) {
    element = Element(static_cast<Substance>(rand() % kSubstanceCount));
  }
}

World::World(const sf::Vector2u size) : World(sf::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y))) {}

Element World::GetElementAt(const size_t index) const {
  return elements_[index];
}

Element World::GetElementAt(const sf::Vector2i pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetElementAt(index);
}

const std::vector<sf::Color> kColors{
    sf::Color(150, 0, 0),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs1)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs2)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs3)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs4)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs5)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs6)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs7)),
};

sf::Color World::GetColorAt(const size_t index) const {
  return kColors[static_cast<int>(GetElementAt(index).GetSubstance())];
}

sf::Color World::GetColorAt(sf::Vector2i pos) const {
  if (pos.x < 0 || pos.y < 0 || pos.x >= size_.x || pos.y >= size_.y) {
    pos /= 4;

    if ((pos.x + pos.y) & 1) {
      return {100, 0, 100};  //NOLINT
    }
    return {0, 0, 0};  //NOLINT
  }
  const size_t index = pos.y * size_.x + pos.x;
  return GetColorAt(index);
}
