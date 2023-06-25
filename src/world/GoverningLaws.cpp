#include <SFML/System/Vector2.hpp>
#include <cstdio>

#include "CommonConstants.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

template <>
void World::GovernLaw<engine::Substance::kAir>(const Element /*element*/, const sf::Vector2i /*position*/) {}

template <>
void World::GovernLaw<engine::Substance::kSand>(const Element /*element*/, const sf::Vector2i position) {
  if (position.x == 0 || position.x == constants::kWorldWidth - 1 || position.y == constants::kWorldHeight - 1) {
    return;
  }

  // const size_t index = position.y * constants::kWorldWidth + position.x;

  if (!engine::IsSolid(GetElementAt(position - sf::Vector2i(0, 1)).GetSubstance())) {}
}

template <>
void World::GovernLaw<engine::Substance::kStone>(const Element /*element*/, const sf::Vector2i /*position*/) {}

template <>
void World::GovernLaw<engine::Substance::kWater>(const Element /*element*/, const sf::Vector2i /*position*/) {}

void World::GovernLaw(const sf::Vector2i position) {
  const Element element = GetElementAt(position);

  switch (element.GetSubstance()) {
    case engine::Substance::kAir: {
      GovernLaw<engine::Substance::kAir>(element, position);
      break;
    }
    case engine::Substance::kSand: {
      GovernLaw<engine::Substance::kSand>(element, position);
      break;
    }
    case engine::Substance::kStone: {
      GovernLaw<engine::Substance::kStone>(element, position);
      break;
    }
    case engine::Substance::kWater: {
      GovernLaw<engine::Substance::kWater>(element, position);
      break;
    }
    default: {
      // unreachable (hopefully)
    }
  }
}
