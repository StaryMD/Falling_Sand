#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>

enum class Substance : uint8_t {
  kNothing = 0,
  kSubs1 = 1,
  kSubs2 = 2,
  kSubs3 = 3,
  kSubs4 = 4,
  kSubs5 = 5,
  kSubs6 = 6,
  kSubs7 = 7,
};

constexpr int kSubstanceCount = 7;

constexpr bool IsSolid(Substance);

#endif /* SUBSTANCE_HPP_ */
