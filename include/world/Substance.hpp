#ifndef SUBSTANCE_HPP_
#define SUBSTANCE_HPP_

#include <cstdint>
#include <vector>

#include <SFML/Graphics/Color.hpp>

namespace engine {

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

sf::Color ColorOf(Substance substance);

constexpr bool IsSolid(Substance);

}  // namespace engine

#endif /* SUBSTANCE_HPP_ */
