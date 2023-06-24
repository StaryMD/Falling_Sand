#include "world/Substance.hpp"

namespace engine {

constexpr bool IsSolid(const Substance subs) {
  switch (subs) {
    case Substance::kNothing:
    case Substance::kAir:
    case Substance::kSand:
    case Substance::kStone:
    case Substance::kWater:
      return true;
    case Substance::kSubstanceCount:
      return false;
  }
}

}  // namespace engine
