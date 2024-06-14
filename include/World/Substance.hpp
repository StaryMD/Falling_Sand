#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>

namespace engine {

enum class Substance : uint8_t {
  kNothing = 0,
  kAir = 1,
  kSand = 2,
  kStone = 3,
  kWater = 4,
  kOil = 5,
  kSteam = 6,
  kFire = 7,
  kSmoke = 8,

  kSubstanceCount,
  kInvalid,
};

/**
 * @brief Check whether the substance is solid.
 * 
 * @return bool Whether the substance is solid.
 */
bool IsSolid(Substance);

/**
 * @brief Check whether the substance is movable.
 * 
 * @return bool Whether the substance is movable.
 */
bool IsMovable(Substance);

/**
 * @brief Get the density of the substance.
 * 
 * @return bool Value of the density of the substance.
 */
int8_t GetDensity(Substance);

/**
 * @brief Get the change to ignore the visited property of the chunk manager.
 * 
 * @return bool Value of the change to ignore the visited property of the chunk manager.
 */
int8_t GetChanceToIgnoreVisitedness(Substance);

/**
 * @brief Get the maximum number of cells the substance can travel horizontally.
 * 
 * @return bool Value of the maximum number of cells the substance can travel horizontally.
 */
int8_t GetHorizontalTravel(Substance);

}  // namespace engine

#endif  // SUBSTANCE_HPP_
