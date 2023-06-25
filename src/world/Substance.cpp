#include "world/Substance.hpp"

#include <array>

namespace engine {

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsSolidVector = {
    true, false, true, true, false,
};

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsMovableVector = {
    false, true, true, true, true,
};

bool IsSolid(const Substance subs) {
  return kIsSolidVector[static_cast<int>(subs)];
}

bool IsMovable(const Substance subs) {
  return kIsMovableVector[static_cast<int>(subs)];
}

}  // namespace engine
