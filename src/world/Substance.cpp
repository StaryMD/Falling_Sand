#include "world/Substance.hpp"

#include <array>

namespace engine {

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsSolidVector = {
    true, false, true, true, false,
};

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsMovableVector = {
    false, true, true, true, true,
};

const std::array<int8_t, static_cast<int>(Substance::kSubstanceCount)> kDensityVector = {0, 1, 3, 4, 2};

bool IsSolid(const Substance subs) {
  return kIsSolidVector[static_cast<int>(subs)];
}

bool IsMovable(const Substance subs) {
  return kIsMovableVector[static_cast<int>(subs)];
}

int8_t GetDensity(const Substance subs) {
  return kDensityVector[static_cast<int>(subs)];
}

}  // namespace engine
