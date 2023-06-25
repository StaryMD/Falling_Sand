#include "world/World.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"

World::World(const sf::Vector2i size) : size_(size) {
  elements_.resize(static_cast<size_t>(size_.x) * size_.y);
  for (Element& element : elements_) {
    element = Element(engine::Substance::kAir);
  }
}

World::World(const sf::Vector2u size) : World(sf::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y))) {}

void World::Update() {
  for (int pos_y = 0; pos_y < size_.y; ++pos_y) {
    for (int pos_x = 0; pos_x < size_.x; ++pos_x) {
      GovernLaw(sf::Vector2i(pos_x, pos_y));
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
}

sf::Color World::GetColorAt(const size_t index) const {
  return GetElementAt(index).GetColor();
}

sf::Color World::GetColorAt(const sf::Vector2i pos) const {
  const size_t index = pos.y * size_.x + pos.x;
  return GetColorAt(index);
}
