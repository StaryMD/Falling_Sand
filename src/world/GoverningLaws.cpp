#include <vector>

#include <SFML/System/Vector2.hpp>

#include "RandomNumberGenerators.hpp"
#include "World/Element.hpp"
#include "World/Substance.hpp"
#include "World/World.hpp"

const std::vector<sf::Vector2<int32_t>> kRelativePositions = {sf::Vector2<int32_t>(-1, 0), sf::Vector2<int32_t>(1, 0),
                                                              sf::Vector2<int32_t>(0, -1), sf::Vector2<int32_t>(0, 1)};

template <>
bool World::GovernLaw<engine::Substance::kAir>(Element& /*element*/, const sf::Vector2<int32_t> /*position*/) {
  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kSand>(Element& element, const sf::Vector2<int32_t> position) {
  {  // Check if fancy interactions are available
  }

  if (TryToGoDown<engine::Substance::kSand>(element, position)) {
    return true;
  }

  if (TryToSplatter<engine::Substance::kSand>(element, position)) {
    return true;
  }

  if (TryToFlowDown<engine::Substance::kSand>(element, position)) {
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
  {  // Check if fancy interactions are available
    bool did_something = false;

    for (const sf::Vector2<int32_t> rel_pos : kRelativePositions) {
      const sf::Vector2<int32_t> neigh_position = position + rel_pos;

      if (CanAccess(neigh_position)) {
        Element& neigh_element = GetElementAt(neigh_position);

        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            element = Element(engine::Substance::kSteam);
            neigh_element = Element(engine::Substance::kAir);

            SetVisit(neigh_position, true);
            SetVisit(position, true);

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

  if (TryToGoDown<engine::Substance::kWater>(element, position)) {
    return true;
  }

  element.StopFall();

  if (TryToFlowDown<engine::Substance::kWater>(element, position)) {
    return true;
  }

  if (TryToFlow<engine::Substance::kWater>(element, position)) {
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kOil>(Element& element, const sf::Vector2<int32_t> position) {
  {  // Check if fancy interactions are available
    bool did_something = false;

    for (const sf::Vector2<int32_t> rel_pos : kRelativePositions) {
      const sf::Vector2<int32_t> neigh_position = position + rel_pos;

      if (CanAccess(neigh_position)) {
        const Element& neigh_element = GetElementAt(neigh_position);

        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            if (fastest_rng_.NextInt(5) == 0) {
              element = Element(engine::Substance::kFire);

              SetVisit(neigh_position, true);
              SetVisit(position, true);

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

  if (TryToGoDown<engine::Substance::kOil>(element, position)) {
    return true;
  }

  element.StopFall();

  if (TryToFlowDown<engine::Substance::kOil>(element, position)) {
    return true;
  }

  if (TryToFlow<engine::Substance::kOil>(element, position)) {
    return true;
  }

  return false;
}

template <>
bool World::GovernLaw<engine::Substance::kSteam>(Element& element, const sf::Vector2<int32_t> position) {
  {  // Check if fancy interactions are available
    bool did_something = false;

    for (const sf::Vector2<int32_t> rel_pos : kRelativePositions) {
      const sf::Vector2<int32_t> neigh_position = position + rel_pos;

      if (CanAccess(neigh_position)) {
        Element& neigh_element = GetElementAt(neigh_position);

        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kFire: {
            element = Element(engine::Substance::kWater);
            neigh_element = Element(engine::Substance::kAir);

            SetVisit(neigh_position, true);
            SetVisit(position, true);

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

  // Check if it can condense into water
  if ((AirNeighbourCount(position) > 0) && (fastest_rng_.NextInt(100) == 0)) {
    element = Element(engine::Substance::kWater);
    SetVisit(position, true);
    return true;
  }

  if (TryToGoUp<engine::Substance::kSteam>(element, position)) {
    return true;
  }

  element.StopFall();

  if (TryToFlowUp<engine::Substance::kSteam>(element, position)) {
    return true;
  }

  if (TryToFlow<engine::Substance::kSteam>(element, position)) {
    return true;
  }

  // Update next time so the chance to condenses into water remains valid
  return true;
}

template <>
bool World::GovernLaw<engine::Substance::kFire>(Element& element, const sf::Vector2<int32_t> position) {
  {  // Check if fancy interactions are available
    bool did_something = false;

    for (const sf::Vector2<int32_t> relative_position : kRelativePositions) {
      const sf::Vector2<int32_t> neigh_position = position + relative_position;

      if (CanAccess(neigh_position)) {
        Element& neigh_element = GetElementAt(neigh_position);

        switch (neigh_element.GetSubstance()) {
          case engine::Substance::kWater: {
            element = Element(engine::Substance::kAir);
            neigh_element = Element(engine::Substance::kSteam);

            SetVisit(neigh_position, true);
            SetVisit(position, true);

            did_something = true;
            break;
          }
          case engine::Substance::kOil: {
            if (fastest_rng_.NextInt(5) == 0) {
              neigh_element = Element(engine::Substance::kFire);

              SetVisit(neigh_position, true);
              SetVisit(position, true);

              did_something = true;
            }
            break;
          }
          case engine::Substance::kSteam: {
            element = Element(engine::Substance::kAir);
            neigh_element = Element(engine::Substance::kWater);

            SetVisit(neigh_position, true);
            SetVisit(position, true);

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
  if ((AirNeighbourCount(position) > 0) && (fastest_rng_.NextInt(100) == 0)) {
    element = Element(engine::Substance::kSmoke);
    SetVisit(position, true);
  }

  return true;
}

template <>
bool World::GovernLaw<engine::Substance::kSmoke>(Element& element, const sf::Vector2<int32_t> position) {
  {  // Check if fancy interactions are available
  }

  if (TryToGoUp<engine::Substance::kSmoke>(element, position)) {
    return true;
  }

  element.StopFall();

  if (TryToFlowUp<engine::Substance::kSmoke>(element, position)) {
    return true;
  }

  if (TryToFlow<engine::Substance::kSmoke>(element, position)) {
    return true;
  }

  return false;
}
