#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>

enum class Substance : uint8_t {
  kNothing = 0,
  kSand = 1,
};

constexpr bool IsSolid(Substance);

#endif /* SUBSTANCE_HPP_ */
