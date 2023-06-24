#include "world/Substance.hpp"
#include <array>

namespace engine {

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsSolidVector = {
    true, false, true, true, false,
};

bool IsSolid(const Substance subs) {
  return kIsSolidVector[static_cast<int>(subs)];
}

}  // namespace engine
