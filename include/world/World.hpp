#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <cmath>
#include <cstddef>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CommonConstants.hpp"
#include "CommonUtility.hpp"
#include "RandomNumberGenerators.hpp"
#include "World/ChunkManager.hpp"
#include "World/Element.hpp"

class World {
 public:
  World() = delete;

  explicit World(sf::Vector2<uint32_t> size);

  [[nodiscard]] sf::Vector2<int32_t> GetSize() const;

  [[nodiscard]] Element& GetElementAt(sf::Vector2<int32_t> pos);

  [[nodiscard]] const Element& GetElementAt(sf::Vector2<int32_t> pos) const;

  [[nodiscard]] engine::Substance GetSubstanceAt(sf::Vector2<int32_t> pos) const;

  void SetElementAt(sf::Vector2<int32_t> pos, Element element);

  void SwapElements(sf::Vector2<int32_t> position1, sf::Vector2<int32_t> position2);

  void Update();

  void SetChunkActivity();

  [[nodiscard]] bool IsChunkActive(sf::Vector2<int32_t> position) const;

  [[nodiscard]] size_t GetElementCount() const;

  [[nodiscard]] void* GetElementsPointer();

  [[nodiscard]] uint32_t GetChunksUpdatedCount() const;

  [[nodiscard]] bool GetVisit(sf::Vector2<int32_t> position) const;

  void SetVisit(sf::Vector2<int32_t> position, bool visit);

  //NOLINTNEXTLINE
  int32_t update_threads_;

 private:
  sf::Vector2<int32_t> size_;

  ChunkManager chunk_manager_;
  std::vector<Element> elements_;
  std::vector<bool> visited_;
  FastRng rng_;
  Counter<uint32_t> fastest_rng_;

  uint32_t chunks_updated_count_;

  void UpdateChunkNeighborhood(int32_t chunk_x, int32_t chunk_y);

  [[nodiscard]] bool CanAccess(sf::Vector2<int32_t> position) const;

  [[nodiscard]] bool CanAccessWithRandomVisit(sf::Vector2<int32_t> position, engine::Substance original_subs);

  [[nodiscard]] uint8_t AirNeighbourCount(sf::Vector2<int32_t> position) const;

  bool GovernLaw(sf::Vector2<int32_t> position);

  template <engine::Substance substance>
  bool GovernLaw(Element& element, sf::Vector2<int32_t> position);

  template <engine::Substance substance>
  [[nodiscard]] bool CanSwap(const sf::Vector2<int32_t> new_position) {
    switch (substance) {
      case engine::Substance::kSand: {
        return CanAccess(new_position) && not engine::IsSolid(GetSubstanceAt(new_position));
      }
      case engine::Substance::kWater:
      case engine::Substance::kOil: {
        return CanAccess(new_position) &&
               (engine::GetDensity(substance) > engine::GetDensity(GetSubstanceAt(new_position)));
      }
      case engine::Substance::kSteam:
      case engine::Substance::kSmoke: {
        return CanAccess(new_position) &&
               (engine::GetDensity(substance) < engine::GetDensity(GetSubstanceAt(new_position)));
      }
      default: {
        // unreachable
      }
    }
  }

  /**
   * @brief Attempt to go down.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToGoDown(Element& element, const sf::Vector2<int32_t> position) {
    const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

    int32_t go_down_tiles = 0;
    for (int32_t i = 1; i <= max_fall_distance; ++i) {
      const bool can_go_down = CanSwap<substance>(DownBy(position, i));

      if (can_go_down) {
        go_down_tiles = i;
      } else {
        break;
      }
    }

    const bool can_go_down = go_down_tiles > 0;

    if (can_go_down) {
      element.GravityAffect();
      SwapElements(position, DownBy(position, go_down_tiles));
      return true;
    }

    return false;
  }

  /**
   * @brief Attempt to go up.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToGoUp(Element& element, const sf::Vector2<int32_t> position) {
    const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));

    int32_t go_up_tiles = 0;
    for (int32_t i = 1; i <= max_fall_distance; ++i) {
      const bool can_go_up = CanSwap<substance>(UpBy(position, i));

      if (can_go_up) {
        go_up_tiles = i;
      } else {
        break;
      }
    }

    const bool can_go_up = go_up_tiles > 0;

    if (can_go_up) {
      element.GravityAffect();
      SwapElements(position, UpBy(position, go_up_tiles));
      return true;
    }

    return false;
  }

  /**
   * @brief Attempt to splatter - after hitting the ground too hard, element should be knocked horizontally.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToSplatter(Element& element, const sf::Vector2<int32_t> position) {
    if (element.GetVerticalSpeed() > constants::kDefaultVerticalSpeed) {
      const int32_t max_fall_distance = static_cast<int32_t>(std::ceil(element.GetVerticalSpeed()));
      const int32_t horizontal_force = std::max(0, max_fall_distance - 2);

      element.StopFall();

      int32_t go_left_tiles = 0;
      for (int32_t i = 1; i <= horizontal_force; ++i) {
        const bool can_go_left = CanSwap<substance>(LeftBy(position, i));

        if (can_go_left) {
          go_left_tiles = i;
        } else {
          break;
        }
      }

      int32_t go_right_tiles = 0;
      for (int32_t i = 1; i <= horizontal_force; ++i) {
        const bool can_go_right = CanSwap<substance>(RightBy(position, i));

        if (can_go_right) {
          go_right_tiles = i;
        } else {
          break;
        }
      }

      const bool can_go_left = go_left_tiles > 0;
      const bool can_go_right = go_right_tiles > 0;

      if (can_go_left && !can_go_right) {
        SwapElements(position, LeftBy(position, go_left_tiles));
        return true;
      }
      if (can_go_right && !can_go_left) {
        SwapElements(position, RightBy(position, go_right_tiles));
        return true;
      }
      if (can_go_left && can_go_right) {
        if (fastest_rng_.NextValue() & 1) {
          SwapElements(position, LeftBy(position, go_left_tiles));
        } else {
          SwapElements(position, RightBy(position, go_right_tiles));
        }
        return true;
      }
      return true;
    }

    return false;
  }

  /**
   * @brief Attempt to flow down - try going down and left/right.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToFlowDown(Element& element, const sf::Vector2<int32_t> position) {
    const bool can_fall_left = CanSwap<substance>(LeftBy(DownBy(position))) && CanSwap<substance>(LeftBy(position));
    const bool can_fall_right = CanSwap<substance>(RightBy(DownBy(position))) && CanSwap<substance>(RightBy(position));

    if (can_fall_left && !can_fall_right) {
      element.SetSpeed(-1);
      SwapElements(position, LeftBy(DownBy(position)));
      return true;
    }
    if (can_fall_right && !can_fall_left) {
      element.SetSpeed(1);
      SwapElements(position, RightBy(DownBy(position)));
      return true;
    }
    if (can_fall_left && can_fall_right) {
      if (element.GetSpeed() < 0) {
        SwapElements(position, LeftBy(DownBy(position)));
      } else {
        SwapElements(position, RightBy(DownBy(position)));
      }
      return true;
    }

    return false;
  }

  /**
   * @brief Attempt to flow up - try going up and left/right.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToFlowUp(Element& element, const sf::Vector2<int32_t> position) {
    const bool can_fall_left = CanSwap<substance>(LeftBy(UpBy(position))) && CanSwap<substance>(LeftBy(position));
    const bool can_fall_right = CanSwap<substance>(RightBy(UpBy(position))) && CanSwap<substance>(RightBy(position));

    if (can_fall_left && !can_fall_right) {
      element.SetSpeed(-1);
      SwapElements(position, LeftBy(UpBy(position)));
      return true;
    }
    if (can_fall_right && !can_fall_left) {
      element.SetSpeed(1);
      SwapElements(position, RightBy(UpBy(position)));
      return true;
    }
    if (can_fall_left && can_fall_right) {
      if (element.GetSpeed() < 0) {
        SwapElements(position, LeftBy(UpBy(position)));
      } else {
        SwapElements(position, RightBy(UpBy(position)));
      }
      return true;
    }

    return false;
  }

  /**
   * @brief Attempt to flow horizontally - try to go left/right.
   * 
   * @param element Element affected.
   * @param position Position of the element affected.
   * @return bool Whether it succeeded to move the element.
   */
  template <engine::Substance substance>
  [[nodiscard]] bool TryToFlow(Element& element, const sf::Vector2<int32_t> position) {
    int32_t go_left_tiles = 0;
    for (int32_t i = 1; i <= engine::GetHorizontalTravel(substance); ++i) {
      const bool can_go_left = CanSwap<substance>(LeftBy(position, i));

      if (can_go_left) {
        go_left_tiles = i;
      } else {
        break;
      }
    }

    int32_t go_right_tiles = 0;
    for (int32_t i = 1; i <= engine::GetHorizontalTravel(substance); ++i) {
      const bool can_go_right = CanSwap<substance>(RightBy(position, i));

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
      SwapElements(position, LeftBy(position, go_left_tiles));
      return true;
    }
    if (can_go_right && !can_go_left) {
      element.SetSpeed(1);
      SwapElements(position, RightBy(position, go_right_tiles));
      return true;
    }
    if (can_go_left && can_go_right) {
      if (element.GetSpeed() < 0) {
        SwapElements(position, LeftBy(position, go_left_tiles));
      } else {
        SwapElements(position, RightBy(position, go_right_tiles));
      }
      return true;
    }

    return false;
  }
};

#endif /* WORLD_HPP_ */
