#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>
#include <vector>

#include <SFML/Graphics/Color.hpp>

namespace engine {

enum class Substance : uint8_t {
  kNothing = 0,
  kAir = 1,
  kSand = 2,
  kStone = 3,
  kWater = 4,

  kSubstanceCount,
  kInvalid,
};

bool IsSolid(Substance);

}  // namespace engine

#endif /* SUBSTANCE_HPP_ */
