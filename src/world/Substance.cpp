#include "world/Substance.hpp"

#include <array>
#include <cstdint>

namespace engine {

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsSolidVector = {
    true, false, true, true, false, false, false,
};

const std::array<bool, static_cast<int>(Substance::kSubstanceCount)> kIsMovableVector = {
    false, true, true, true, true, true, true,
};

const std::array<int8_t, static_cast<int>(Substance::kSubstanceCount)> kDensityVector = {0, 1, 5, 6, 4, 3, 2};

const std::array<int8_t, static_cast<int>(Substance::kSubstanceCount)> kChanceToIgnoreVisitednessVector = {
    -1, -1, -1, -1, 5, 10, 2};

const std::array<int8_t, static_cast<int>(Substance::kSubstanceCount)> kHorizontalTravelVector = {1, 1, 1, 1, 4, 1, 5};

bool IsSolid(const Substance subs) {
  return kIsSolidVector[static_cast<int>(subs)];
}

bool IsMovable(const Substance subs) {
  return kIsMovableVector[static_cast<int>(subs)];
}

int8_t GetDensity(const Substance subs) {
  return kDensityVector[static_cast<int>(subs)];
}

int8_t GetChanceToIgnoreVisitedness(const Substance subs) {
  return kChanceToIgnoreVisitednessVector[static_cast<int>(subs)];
}

int8_t GetHorizontalTravel(const Substance subs) {
  return kHorizontalTravelVector[static_cast<int>(subs)];
}

}  // namespace engine
