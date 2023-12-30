#include "World/Substance.hpp"

#include <array>
#include <cstdint>

namespace engine {

const std::array<bool, static_cast<int32_t>(Substance::kSubstanceCount)> kIsSolidVector = {
    true,   // Nothing
    false,  // Air
    true,   // Sand
    true,   // Stone
    false,  // Water
    false,  // Oil
    false,  // Steam
    false,  // Fire
    false,  // Smoke

};

const std::array<bool, static_cast<int32_t>(Substance::kSubstanceCount)> kIsMovableVector = {
    false,  // Nothing
    true,   // Air
    true,   // Sand
    false,  // Stone
    true,   // Water
    true,   // Oil
    true,   // Steam
    false,  // Fire
    true,   // Smoke

};

const std::array<int8_t, static_cast<int32_t>(Substance::kSubstanceCount)> kDensityVector = {
    0,  // Nothing
    3,  // Air
    6,  // Sand
    7,  // Stone
    5,  // Water
    4,  // Oil
    2,  // Steam
    0,  // Fire
    1,  // Smoke
};

const std::array<int8_t, static_cast<int32_t>(Substance::kSubstanceCount)> kChanceToIgnoreVisitednessVector = {
    -1,  // Nothing
    -1,  // Air
    -1,  // Sand
    -1,  // Stone
    5,   // Water
    10,  // Oil
    2,   // Steam
    -1,  // Fire
    -1,  // Smoke

};

const std::array<int8_t, static_cast<int32_t>(Substance::kSubstanceCount)> kHorizontalTravelVector = {
    1,   // Nothing
    1,   // Air
    1,   // Sand
    1,   // Stone
    4,   // Water
    1,   // Oil
    5,   // Steam
    -1,  // Fire
    2,   // Smoke

};

bool IsSolid(const Substance subs) {
  return kIsSolidVector[static_cast<int32_t>(subs)];
}

bool IsMovable(const Substance subs) {
  return kIsMovableVector[static_cast<int32_t>(subs)];
}

int8_t GetDensity(const Substance subs) {
  return kDensityVector[static_cast<int32_t>(subs)];
}

int8_t GetChanceToIgnoreVisitedness(const Substance subs) {
  return kChanceToIgnoreVisitednessVector[static_cast<int32_t>(subs)];
}

int8_t GetHorizontalTravel(const Substance subs) {
  return kHorizontalTravelVector[static_cast<int32_t>(subs)];
}

}  // namespace engine
