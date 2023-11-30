#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>

#include <SFML/Graphics/Color.hpp>

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

bool IsSolid(Substance);

bool IsMovable(Substance);

int8_t GetDensity(Substance);

int8_t GetChanceToIgnoreVisitedness(Substance);

int8_t GetHorizontalTravel(Substance);

}  // namespace engine

#endif /* SUBSTANCE_HPP_ */
