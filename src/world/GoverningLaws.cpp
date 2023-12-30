#include <cmath>

#include <algorithm>

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "CommonConstants.hpp"
#include "RandomNumberGenerators.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"
#include "World/World.hpp"

const std::vector<sf::Vector2<int32_t>> kRelPositions = {sf::Vector2<int32_t>(-1, 0), sf::Vector2<int32_t>(1, 0),
                                                         sf::Vector2<int32_t>(0, -1), sf::Vector2<int32_t>(0, 1)};

template <>
bool World::GovernLaw<engine::Substance::kAir>(Element& /*element*/, const sf::Vector2<int32_t> /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kSand>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
  }

  const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

  int32_t go_down_tiles = 0;
  for (int32_t i = 1; i <= max_fall_distance; ++i) {
    const bool can_go_down = CanAccess({position.x, position.y + i}) &&
                             !engine::IsSolid(GetElementAt({position.x, position.y + i}).GetSubstance());

    if (can_go_down) {
      go_down_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_down = go_down_tiles > 0;

  if (can_go_down) {
    element.GravityAffect();
    SwapElements(index, index + constants::kWorldWidth * go_down_tiles);
    return true;
  }

  // Hit the ground and has force left, knock it left or right
  if (element.GetVerticalSpeed() > 0.0F) {
    element.StopFall();
    const int32_t horizontal_force = std::max(0, max_fall_distance - 2);

    int32_t go_left_tiles = 0;
    for (int32_t i = 1; i <= horizontal_force; ++i) {
      const bool can_go_left =
          CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kSand) &&
          (engine::GetDensity(engine::Substance::kSand) > engine::GetDensity(GetElementAt(index - i).GetSubstance()));

      if (can_go_left) {
        go_left_tiles = i;
      } else {
        break;
      }
    }

    int32_t go_right_tiles = 0;
    for (int32_t i = 1; i <= horizontal_force; ++i) {
      const bool can_go_right =
          CanAccessWithRandomVisit({position.x + i, position.y}, engine::Substance::kSand) &&
          (engine::GetDensity(engine::Substance::kSand) > engine::GetDensity(GetElementAt(index + i).GetSubstance()));

      if (can_go_right) {
        go_right_tiles = i;
      } else {
        break;
      }
    }

    const bool can_go_left = go_left_tiles > 0;
    const bool can_go_right = go_right_tiles > 0;

    if (can_go_left && !can_go_right) {
      SwapElements(index, index - go_left_tiles);
      return true;
    }
    if (can_go_right && !can_go_left) {
      SwapElements(index, index + go_right_tiles);
      return true;
    }
    if (can_go_left && can_go_right) {
      if (fastest_rng_.NextValue() & 1) {
        SwapElements(index, index - go_left_tiles);
      } else {
        SwapElements(index, index + go_right_tiles);
      }
      return true;
    }
  }

  element.StopFall();

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
    if (fastest_rng_.NextValue() & 1) {
      SwapElements(index, index + constants::kWorldWidth - 1);
    } else {
      SwapElements(index, index + constants::kWorldWidth + 1);
    }
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kStone>(Element& /*element*/, const sf::Vector2<int32_t> /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kWater>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
    bool did_something = false;

    for (const auto rel_pos : kRelPositions) {
      const sf::Vector2<int32_t> neigh_pos = position + rel_pos;
      const int32_t neigh_index = neigh_pos.y * constants::kWorldWidth + neigh_pos.x;
      Element& neigh_element = GetElementAt(neigh_pos);

      if (CanAccess(neigh_pos)) {
        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            element = Element(engine::Substance::kSteam);
            neigh_element = Element(engine::Substance::kAir);

            visited_[neigh_index] = visited_[index] = true;
            did_something = true;
            break;
          }
          default: {
          }
        }
      }
    }

    if (did_something) {
      return true;
    }
  }

  const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

  int32_t go_down_tiles = 0;
  for (int32_t i = 1; i <= max_fall_distance; ++i) {
    const bool can_go_down = CanAccess({position.x, position.y + i}) &&
                             (engine::GetDensity(engine::Substance::kWater) >
                              engine::GetDensity(GetElementAt(index + constants::kWorldWidth * i).GetSubstance()));

    if (can_go_down) {
      go_down_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_down = go_down_tiles > 0;

  if (can_go_down) {
    element.GravityAffect();
    SwapElements(index, index + constants::kWorldWidth * go_down_tiles);
    return true;
  }

  element.StopFall();

  const bool can_fall_left =
      (CanAccess({position.x - 1, position.y + 1}) &&
       (engine::GetDensity(engine::Substance::kWater) >
        engine::GetDensity(GetElementAt(index + constants::kWorldWidth - 1).GetSubstance()))) &&
      (CanAccess({position.x - 1, position.y}) && not engine::IsSolid(GetElementAt(index - 1).GetSubstance()));
  const bool can_fall_right =
      (CanAccess({position.x + 1, position.y + 1}) &&
       (engine::GetDensity(engine::Substance::kWater) >
        engine::GetDensity(GetElementAt(index + constants::kWorldWidth + 1).GetSubstance()))) &&
      (CanAccess({position.x + 1, position.y}) && not engine::IsSolid(GetElementAt(index + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    element.SetSpeed(-1);
    SwapElements(index, index + constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    element.SetSpeed(1);
    SwapElements(index, index + constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (fastest_rng_.NextValue() & 1) {
      element.SetSpeed(-1);
      SwapElements(index, index + constants::kWorldWidth - 1);
    } else {
      element.SetSpeed(1);
      SwapElements(index, index + constants::kWorldWidth + 1);
    }
    return true;
  }

  int32_t go_left_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kWater); ++i) {
    const bool can_go_left =
        CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kWater) &&
        (engine::GetDensity(engine::Substance::kWater) > engine::GetDensity(GetElementAt(index - i).GetSubstance()));

    if (can_go_left) {
      go_left_tiles = i;
    } else {
      break;
    }
  }

  int32_t go_right_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kWater); ++i) {
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
    element.SetSpeed(-1);
    SwapElements(index, index - go_left_tiles);
    return true;
  }
  if (can_go_right && !can_go_left) {
    element.SetSpeed(1);
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

template <>
bool World::GovernLaw<engine::Substance::kOil>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
    bool did_something = false;

    for (const auto rel_pos : kRelPositions) {
      const sf::Vector2<int32_t> neigh_pos = position + rel_pos;
      const int32_t neigh_index = neigh_pos.y * constants::kWorldWidth + neigh_pos.x;
      const Element& neigh_element = GetElementAt(neigh_pos);

      if (CanAccess(neigh_pos)) {
        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            if (fastest_rng_.NextInt(5) == 0) {
              element = Element(engine::Substance::kFire);

              visited_[neigh_index] = visited_[index] = true;
              did_something = true;
            }

            break;
          }
          default: {
          }
        }
      }
    }

    if (did_something) {
      return true;
    }
  }

  const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

  int32_t go_down_tiles = 0;
  for (int32_t i = 1; i <= max_fall_distance; ++i) {
    const bool can_go_down = CanAccess({position.x, position.y + i}) &&
                             (engine::GetDensity(engine::Substance::kOil) >
                              engine::GetDensity(GetElementAt(index + constants::kWorldWidth * i).GetSubstance()));

    if (can_go_down) {
      go_down_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_down = go_down_tiles > 0;

  if (can_go_down) {
    element.GravityAffect();
    SwapElements(index, index + constants::kWorldWidth * go_down_tiles);
    return true;
  }

  element.StopFall();

  const bool can_fall_left =
      (CanAccess({position.x - 1, position.y + 1}) &&
       (engine::GetDensity(engine::Substance::kOil) >
        engine::GetDensity(GetElementAt(index + constants::kWorldWidth - 1).GetSubstance()))) &&
      (CanAccess({position.x - 1, position.y}) && not engine::IsSolid(GetElementAt(index - 1).GetSubstance()));
  const bool can_fall_right =
      (CanAccess({position.x + 1, position.y + 1}) &&
       (engine::GetDensity(engine::Substance::kOil) >
        engine::GetDensity(GetElementAt(index + constants::kWorldWidth + 1).GetSubstance()))) &&
      (CanAccess({position.x + 1, position.y}) && not engine::IsSolid(GetElementAt(index + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    element.SetSpeed(-1);
    SwapElements(index, index + constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    element.SetSpeed(1);
    SwapElements(index, index + constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (fastest_rng_.NextValue() & 1) {
      element.SetSpeed(-1);
      SwapElements(index, index + constants::kWorldWidth - 1);
    } else {
      element.SetSpeed(1);
      SwapElements(index, index + constants::kWorldWidth + 1);
    }
    return true;
  }

  int32_t go_left_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kOil); ++i) {
    const bool can_go_left =
        CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kOil) &&
        (engine::GetDensity(engine::Substance::kOil) > engine::GetDensity(GetElementAt(index - i).GetSubstance()));

    if (can_go_left) {
      go_left_tiles = i;
    } else {
      break;
    }
  }

  int32_t go_right_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kOil); ++i) {
    const bool can_go_right =
        CanAccessWithRandomVisit({position.x + i, position.y}, engine::Substance::kOil) &&
        (engine::GetDensity(engine::Substance::kOil) > engine::GetDensity(GetElementAt(index + i).GetSubstance()));

    if (can_go_right) {
      go_right_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_left = go_left_tiles > 0;
  const bool can_go_right = go_right_tiles > 0;

  if (can_go_left && !can_go_right) {
    element.SetSpeed(-1);
    SwapElements(index, index - go_left_tiles);
    return true;
  }
  if (can_go_right && !can_go_left) {
    element.SetSpeed(1);
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

template <>
bool World::GovernLaw<engine::Substance::kSteam>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
    bool did_something = false;

    for (const auto rel_pos : kRelPositions) {
      const sf::Vector2<int32_t> neigh_pos = position + rel_pos;
      const int32_t neigh_index = neigh_pos.y * constants::kWorldWidth + neigh_pos.x;
      Element& neigh_element = GetElementAt(neigh_pos);

      if (CanAccess(neigh_pos)) {
        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            element = Element(engine::Substance::kWater);
            neigh_element = Element(engine::Substance::kAir);

            visited_[neigh_index] = visited_[index] = true;
            did_something = true;
            break;
          }
          default: {
          }
        }
      }
    }

    if (did_something) {
      return true;
    }
  }

  // Check if it can condense int32_to water
  if ((AirNeighbourCount(index) > 0) && (fastest_rng_.NextInt(100) == 0)) {
    element = Element(engine::Substance::kWater);
    visited_[index] = true;
    return true;
  }

  const int32_t max_up_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

  int32_t go_down_tiles = 0;
  for (int32_t i = 1; i <= max_up_distance; ++i) {
    const bool can_go_up = CanAccess({position.x, position.y - i}) &&
                           (engine::GetDensity(engine::Substance::kSteam) <
                            engine::GetDensity(GetElementAt(index - constants::kWorldWidth * i).GetSubstance()));

    if (can_go_up) {
      go_down_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_up = go_down_tiles > 0;

  if (can_go_up) {
    element.GravityAffect();
    SwapElements(index, index - constants::kWorldWidth * go_down_tiles);
    return true;
  }

  element.StopFall();

  const bool can_fall_left =
      (CanAccess({position.x - 1, position.y - 1}) &&
       (engine::GetDensity(engine::Substance::kSteam) <
        engine::GetDensity(GetElementAt(index - constants::kWorldWidth - 1).GetSubstance()))) &&
      (CanAccess({position.x - 1, position.y}) && not engine::IsSolid(GetElementAt(index - 1).GetSubstance()));
  const bool can_fall_right =
      (CanAccess({position.x + 1, position.y - 1}) &&
       (engine::GetDensity(engine::Substance::kSteam) <
        engine::GetDensity(GetElementAt(index - constants::kWorldWidth + 1).GetSubstance()))) &&
      (CanAccess({position.x + 1, position.y}) && not engine::IsSolid(GetElementAt(index + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    element.SetSpeed(-1);
    SwapElements(index, index - constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    element.SetSpeed(1);
    SwapElements(index, index - constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (fastest_rng_.NextValue() & 1) {
      element.SetSpeed(-1);
      SwapElements(index, index - constants::kWorldWidth - 1);
    } else {
      element.SetSpeed(1);
      SwapElements(index, index - constants::kWorldWidth + 1);
    }
    return true;
  }

  int32_t go_left_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kSteam); ++i) {
    const bool can_go_left =
        CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kSteam) &&
        (engine::GetDensity(engine::Substance::kSteam) < engine::GetDensity(GetElementAt(index - i).GetSubstance()));

    if (can_go_left) {
      go_left_tiles = i;
    } else {
      break;
    }
  }

  int32_t go_right_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kSteam); ++i) {
    const bool can_go_right =
        CanAccessWithRandomVisit({position.x + i, position.y}, engine::Substance::kSteam) &&
        (engine::GetDensity(engine::Substance::kSteam) < engine::GetDensity(GetElementAt(index + i).GetSubstance()));

    if (can_go_right) {
      go_right_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_left = go_left_tiles > 0;
  const bool can_go_right = go_right_tiles > 0;

  if (can_go_left && !can_go_right) {
    element.SetSpeed(-1);
    SwapElements(index, index - go_left_tiles);
    return true;
  }
  if (can_go_right && !can_go_left) {
    element.SetSpeed(1);
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
  return true;
}

template <>
bool World::GovernLaw<engine::Substance::kFire>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
    bool did_something = false;

    for (const auto rel_pos : kRelPositions) {
      const sf::Vector2<int32_t> neigh_pos = position + rel_pos;
      const int32_t neigh_index = neigh_pos.y * constants::kWorldWidth + neigh_pos.x;
      Element& neigh_element = GetElementAt(neigh_pos);

      if (CanAccess(neigh_pos)) {
        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kWater: {
            element = Element(engine::Substance::kAir);
            neigh_element = Element(engine::Substance::kSteam);

            visited_[neigh_index] = visited_[index] = true;
            did_something = true;
            break;
          }
          case engine::Substance::kOil: {
            if (fastest_rng_.NextInt(5) == 0) {
              neigh_element = Element(engine::Substance::kFire);

              did_something = true;
            }

            visited_[neigh_index] = visited_[index] = true;
            break;
          }
          case engine::Substance::kSteam: {
            element = Element(engine::Substance::kAir);
            neigh_element = Element(engine::Substance::kWater);

            visited_[neigh_index] = visited_[index] = true;
            did_something = true;
            break;
          }
          default: {
          }
        }
      }
    }

    if (did_something) {
      return true;
    }
  }

  // Check if fire can die
  if ((AirNeighbourCount(index) > 0) && (fastest_rng_.NextInt(100) == 0)) {
    element = Element(engine::Substance::kSmoke);
    visited_[index] = true;
  }

  return true;
}

template <>
bool World::GovernLaw<engine::Substance::kSmoke>(Element& element, const sf::Vector2<int32_t> position) {
  const int32_t index = position.y * constants::kWorldWidth + position.x;

  {  // Check if fancy int32_teractions are available
  }

  const int32_t max_up_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

  int32_t go_down_tiles = 0;
  for (int32_t i = 1; i <= max_up_distance; ++i) {
    const bool can_go_up = CanAccess({position.x, position.y - i}) &&
                           (engine::GetDensity(engine::Substance::kSmoke) <
                            engine::GetDensity(GetElementAt(index - constants::kWorldWidth * i).GetSubstance()));

    if (can_go_up) {
      go_down_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_up = go_down_tiles > 0;

  if (can_go_up) {
    element.GravityAffect();
    SwapElements(index, index - constants::kWorldWidth * go_down_tiles);
    return true;
  }

  element.StopFall();

  const bool can_fall_left =
      (CanAccess({position.x - 1, position.y - 1}) &&
       (engine::GetDensity(engine::Substance::kSmoke) <
        engine::GetDensity(GetElementAt(index - constants::kWorldWidth - 1).GetSubstance()))) &&
      (CanAccess({position.x - 1, position.y}) && not engine::IsSolid(GetElementAt(index - 1).GetSubstance()));
  const bool can_fall_right =
      (CanAccess({position.x + 1, position.y - 1}) &&
       (engine::GetDensity(engine::Substance::kSmoke) <
        engine::GetDensity(GetElementAt(index - constants::kWorldWidth + 1).GetSubstance()))) &&
      (CanAccess({position.x + 1, position.y}) && not engine::IsSolid(GetElementAt(index + 1).GetSubstance()));

  if (can_fall_left && !can_fall_right) {
    element.SetSpeed(-1);
    SwapElements(index, index - constants::kWorldWidth - 1);
    return true;
  }
  if (can_fall_right && !can_fall_left) {
    element.SetSpeed(1);
    SwapElements(index, index - constants::kWorldWidth + 1);
    return true;
  }
  if (can_fall_left && can_fall_right) {
    if (fastest_rng_.NextValue() & 1) {
      element.SetSpeed(-1);
      SwapElements(index, index - constants::kWorldWidth - 1);
    } else {
      element.SetSpeed(1);
      SwapElements(index, index - constants::kWorldWidth + 1);
    }
    return true;
  }

  int32_t go_left_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kSmoke); ++i) {
    const bool can_go_left =
        CanAccessWithRandomVisit({position.x - i, position.y}, engine::Substance::kSmoke) &&
        (engine::GetDensity(engine::Substance::kSmoke) < engine::GetDensity(GetElementAt(index - i).GetSubstance()));

    if (can_go_left) {
      go_left_tiles = i;
    } else {
      break;
    }
  }

  int32_t go_right_tiles = 0;
  for (int32_t i = 1; i <= engine::GetHorizontalTravel(engine::Substance::kSmoke); ++i) {
    const bool can_go_right =
        CanAccessWithRandomVisit({position.x + i, position.y}, engine::Substance::kSmoke) &&
        (engine::GetDensity(engine::Substance::kSmoke) < engine::GetDensity(GetElementAt(index + i).GetSubstance()));

    if (can_go_right) {
      go_right_tiles = i;
    } else {
      break;
    }
  }

  const bool can_go_left = go_left_tiles > 0;
  const bool can_go_right = go_right_tiles > 0;

  if (can_go_left && !can_go_right) {
    element.SetSpeed(-1);
    SwapElements(index, index - go_left_tiles);
    return true;
  }
  if (can_go_right && !can_go_left) {
    element.SetSpeed(1);
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

bool World::GovernLaw(const sf::Vector2<int32_t> position) {
  Element& element = GetElementAt(position);

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
    case engine::Substance::kOil: {
      return GovernLaw<engine::Substance::kOil>(element, position);
    }
    case engine::Substance::kSteam: {
      return GovernLaw<engine::Substance::kSteam>(element, position);
    }
    case engine::Substance::kFire: {
      return GovernLaw<engine::Substance::kFire>(element, position);
    }
    case engine::Substance::kSmoke: {
      return GovernLaw<engine::Substance::kSmoke>(element, position);
    }
    default: {
      // unreachable (hopefully)
    }
  }

  return false;
}
