#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "RandomNumberGenerators.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

using constants::kWorldWidth;
using engine::GetDensity;
using engine::IsSolid;
using engine::Substance;

template <>
bool World::GovernLaw<Substance::kAir>(const Element /*element*/, const sf::Vector2i /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<Substance::kSand>(const Element /*element*/, const sf::Vector2i position) {
  if ((position.y == constants::kWorldHeight - 1)) {
    return false;
  }

  const int index = position.y * kWorldWidth + position.x;

  if (!WasNotUpdatedThisTick(GetElementAt(index))) {
    return false;
  }

  if (!IsSolid(GetSubstanceAt(index + kWorldWidth))) {
    SwapElements(index, index + kWorldWidth);
  }

  const bool can_fall_left = position.x > 0 && !IsSolid(GetSubstanceAt(index - 1)) &&
                             !IsSolid(GetSubstanceAt(index + kWorldWidth - 1)) &&
                             WasNotUpdatedThisTick(GetElementAt(index + kWorldWidth - 1));
  const bool can_fall_right = position.x < kWorldWidth - 1 && !IsSolid(GetSubstanceAt(index + 1)) &&
                              !IsSolid(GetSubstanceAt(index + kWorldWidth + 1)) &&
                              WasNotUpdatedThisTick(GetElementAt(index + kWorldWidth + 1));

  if (can_fall_left && !can_fall_right) {
    SwapElements(index, index + kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    SwapElements(index, index + kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (rng_.NextRandValue() & 1) {
      SwapElements(index, index + kWorldWidth - 1);
    } else {
      SwapElements(index, index + kWorldWidth + 1);
    }
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<Substance::kStone>(const Element /*element*/, const sf::Vector2i /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<Substance::kWater>(const Element element, const sf::Vector2i position) {
  const int index = position.y * kWorldWidth + position.x;

  if (!WasNotUpdatedThisTick(GetElementAt(index))) {
    return false;
  }

  const bool can_fall_down = position.y != constants::kWorldHeight - 1 &&
                             (GetDensity(Substance::kWater) > GetDensity(GetSubstanceAt(index + kWorldWidth)) &&
                              WasNotUpdatedThisTick(GetElementAt(index + kWorldWidth)));
  if (can_fall_down) {
    SwapElements(index, index + kWorldWidth);
    return true;
  }

  const bool can_fall_left = position.y != constants::kWorldHeight - 1 && position.x > 0 &&
                             (GetDensity(Substance::kWater) > GetDensity(GetSubstanceAt(index + kWorldWidth - 1)) &&
                              WasNotUpdatedThisTick(GetElementAt(index + kWorldWidth - 1)));
  const bool can_fall_right = position.y != constants::kWorldHeight - 1 && position.x < kWorldWidth - 1 &&
                              (GetDensity(Substance::kWater) > GetDensity(GetSubstanceAt(index + kWorldWidth + 1)) &&
                               WasNotUpdatedThisTick(GetElementAt(index + kWorldWidth + 1)));

  if (can_fall_left && !can_fall_right) {
    elements_[index].SetDirection(0);
    SwapElements(index, index + kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    elements_[index].SetDirection(1);
    SwapElements(index, index + kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (element.GetDirection() == 0) {
      SwapElements(index, index + kWorldWidth - 1);
    } else {
      SwapElements(index, index + kWorldWidth + 1);
    }
    return true;
  }

  const bool can_slide_left =
      position.x > 0 && (GetDensity(Substance::kWater) > GetDensity(GetSubstanceAt(index - 1)) &&
                         WasNotUpdatedThisTick(GetElementAt(index - 1)));
  const bool can_slide_right =
      position.x < kWorldWidth - 1 && (GetDensity(Substance::kWater) > GetDensity(GetSubstanceAt(index + 1)) &&
                                       WasNotUpdatedThisTick(GetElementAt(index + 1)));

  if (can_slide_left && !can_slide_right) {
    elements_[index].SetDirection(0);
    SwapElements(index, index - 1);
    return true;
  }
  if (can_slide_right && !can_slide_left) {
    elements_[index].SetDirection(1);
    SwapElements(index, index + 1);
    return true;
  }
  if (can_slide_left && can_slide_right) {
    if (element.GetDirection() == 0) {
      SwapElements(index, index - 1);
    } else {
      SwapElements(index, index + 1);
    }
    return true;
  }

  return false;
}

bool World::GovernLaw(const sf::Vector2i position) {
  const Element element = GetElementAt(position);

  switch (element.GetSubstance()) {
    case Substance::kAir: {
      return GovernLaw<Substance::kAir>(element, position);
    }
    case Substance::kSand: {
      return GovernLaw<Substance::kSand>(element, position);
    }
    case Substance::kStone: {
      return GovernLaw<Substance::kStone>(element, position);
    }
    case Substance::kWater: {
      return GovernLaw<Substance::kWater>(element, position);
    }
    default: {
      // unreachable (hopefully)
    }
  }

  return false;
}
