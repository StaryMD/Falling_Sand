#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "RandomNumberGenerators.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

template <>
bool World::GovernLaw<engine::Substance::kAir>(const Element /*element*/, const sf::Vector2i position) {
  const int index = position.y * constants::kWorldWidth + position.x;

  const bool can_fall_down = CanAccess({position.x, position.y - 1}) &&
                             (engine::GetDensity(engine::Substance::kAir) >
                              engine::GetDensity(GetElementAt(index - constants::kWorldWidth).GetSubstance()));

  if (can_fall_down) {
    SwapElements(index, index - constants::kWorldWidth);
    return true;
  }

  const bool can_fall_left = CanAccess({position.x - 1, position.y - 1}) &&
                             (engine::GetDensity(engine::Substance::kAir) >
                              engine::GetDensity(GetElementAt(index - constants::kWorldWidth - 1).GetSubstance()));
  const bool can_fall_right = CanAccess({position.x + 1, position.y - 1}) &&
                              (engine::GetDensity(engine::Substance::kAir) >
                               engine::GetDensity(GetElementAt(index - constants::kWorldWidth + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    SwapElements(index, index - constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    SwapElements(index, index - constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (rng_.NextRandValue() & 1) {
      SwapElements(index, index - constants::kWorldWidth - 1);
    } else {
      SwapElements(index, index - constants::kWorldWidth + 1);
    }
    return true;
  }

  const bool can_go_left =
      CanAccessWithRandomVisit({position.x - 1, position.y}, engine::Substance::kAir) &&
      (engine::GetDensity(engine::Substance::kAir) > engine::GetDensity(GetElementAt(index - 1).GetSubstance()));
  const bool can_go_right =
      CanAccessWithRandomVisit({position.x + 1, position.y}, engine::Substance::kAir) &&
      (engine::GetDensity(engine::Substance::kAir) > engine::GetDensity(GetElementAt(index + 1).GetSubstance()));

  if (can_go_left && !can_go_right) {
    SwapElements(index, index - 1);
    return true;
  }
  if (can_go_right && !can_go_left) {
    SwapElements(index, index + 1);
    return true;
  }
  if (can_go_left && can_go_right) {
    if (rng_.NextRandValue() & 1) {
      SwapElements(index, index - 1);
    } else {
      SwapElements(index, index + 1);
    }
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kSand>(const Element /*element*/, const sf::Vector2i position) {
  const int index = position.y * constants::kWorldWidth + position.x;

  if (CanAccess({position.x, position.y + 1}) &&
      !engine::IsSolid(GetElementAt(index + constants::kWorldWidth).GetSubstance())) {
    SwapElements(index, index + constants::kWorldWidth);
    return true;
  }

  const bool can_fall_left = CanAccess({position.x - 1, position.y + 1}) &&
                             !engine::IsSolid(GetElementAt(index - 1).GetSubstance()) &&
                             !engine::IsSolid(GetElementAt(index + constants::kWorldWidth - 1).GetSubstance());
  const bool can_fall_right = CanAccess({position.x + 1, position.y + 1}) &&
                              !engine::IsSolid(GetElementAt(index + 1).GetSubstance()) &&
                              !engine::IsSolid(GetElementAt(index + constants::kWorldWidth + 1).GetSubstance());

  if (can_fall_left && !can_fall_right) {
    SwapElements(index, index + constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    SwapElements(index, index + constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (rng_.NextRandValue() & 1) {
      SwapElements(index, index + constants::kWorldWidth - 1);
    } else {
      SwapElements(index, index + constants::kWorldWidth + 1);
    }
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kStone>(const Element /*element*/, const sf::Vector2i /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kWater>(const Element element, const sf::Vector2i position) {
  const int index = position.y * constants::kWorldWidth + position.x;

  const bool can_fall_down = CanAccess({position.x, position.y + 1}) &&
                             (engine::GetDensity(engine::Substance::kWater) >
                              engine::GetDensity(GetElementAt(index + constants::kWorldWidth).GetSubstance()));

  if (can_fall_down) {
    SwapElements(index, index + constants::kWorldWidth);
    return true;
  }

  const bool can_fall_left = CanAccess({position.x - 1, position.y + 1}) &&
                             (engine::GetDensity(engine::Substance::kWater) >
                              engine::GetDensity(GetElementAt(index + constants::kWorldWidth - 1).GetSubstance()));
  const bool can_fall_right = CanAccess({position.x + 1, position.y + 1}) &&
                              (engine::GetDensity(engine::Substance::kWater) >
                               engine::GetDensity(GetElementAt(index + constants::kWorldWidth + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    elements_[index].SetSpeed(-1);
    SwapElements(index, index + constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    elements_[index].SetSpeed(1);
    SwapElements(index, index + constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (rng_.NextRandValue() & 1) {
      elements_[index].SetSpeed(-1);
      SwapElements(index, index + constants::kWorldWidth - 1);
    } else {
      elements_[index].SetSpeed(1);
      SwapElements(index, index + constants::kWorldWidth + 1);
    }
    return true;
  }

  int go_left_tiles = 0;
  for (int i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kWater); ++i) {
    const bool can_go_left =
        CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kWater) &&
        (engine::GetDensity(engine::Substance::kWater) > engine::GetDensity(GetElementAt(index - i).GetSubstance()));

    if (can_go_left) {
      go_left_tiles = i;
    } else {
      break;
    }
  }

  int go_right_tiles = 0;
  for (int i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kWater); ++i) {
    const bool can_go_right =
        CanAccessWithRandomVisit({position.x + i, position.y}, engine::Substance::kWater) &&
        (engine::GetDensity(engine::Substance::kWater) > engine::GetDensity(GetElementAt(index + i).GetSubstance()));

    if (can_go_right) {
      go_right_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_left = go_left_tiles > 0;
  const bool can_go_right = go_right_tiles > 0;

  if (can_go_left && !can_go_right) {
    elements_[index].SetSpeed(-1);
    SwapElements(index, index - go_left_tiles);
    return true;
  }
  if (can_go_right && !can_go_left) {
    elements_[index].SetSpeed(1);
    SwapElements(index, index + go_right_tiles);
    return true;
  }
  if (can_go_left && can_go_right) {
    if (element.GetSpeed() < 0) {
      SwapElements(index, index - go_left_tiles);
    } else {
      SwapElements(index, index + go_right_tiles);
    }
    return true;
  }

  return false;
}

bool World::GovernLaw(const sf::Vector2i position) {
  const Element element = GetElementAt(position);

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
    default: {
      // unreachable (hopefully)
    }
  }

  return false;
}
