#include "world/Substance.hpp"

namespace engine {

constexpr bool IsSolid(const Substance subs) {
  switch (subs) {
    case Substance::kNothing:
    case Substance::kSubs1:
    case Substance::kSubs2:
    case Substance::kSubs3:
    case Substance::kSubs4:
    case Substance::kSubs5:
    case Substance::kSubs6:
    case Substance::kSubs7:
      return true;
      break;
  }
}

const std::vector<sf::Color> kColors{
    sf::Color(150, 0, 0),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs1)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs2)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs3)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs4)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs5)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs6)),
    sf::Color(255 / kSubstanceCount * static_cast<int>(Substance::kSubs7)),
};

sf::Color ColorOf(const Substance substance) {
  return kColors[static_cast<int>(substance)];
}

}  // namespace engine
