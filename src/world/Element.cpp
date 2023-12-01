#include "world/Element.hpp"

#include <vector>

#include <SFML/Graphics/Color.hpp>

#include "CommonConstants.hpp"
#include "RandomNumberGenerators.hpp"
#include "world/Substance.hpp"

constexpr float kDefaultVerticalSpeed = 0.01F;
constexpr float kMaxVerticalSpeed = 4.0F;

namespace {
FastRng element_rng;
};

Element::Element() : substance_(engine::Substance::kNothing) {
  horizontal_speed_ = -1;
  vertical_speed_ = kDefaultVerticalSpeed;
}

Element::Element(const engine::Substance subs) : substance_(subs) {
  horizontal_speed_ = -1;
  vertical_speed_ = kDefaultVerticalSpeed;

  switch (subs) {
    case engine::Substance::kAir:
    case engine::Substance::kSand:
    case engine::Substance::kStone: {
      draw_property_ = element_rng.NextRandValue() % 4;
      break;
    }
    default: {
    }
  }
}

const std::vector<sf::Color> kColors{
    sf::Color(0x000000FFU), sf::Color(0xD0D0D0FFU), sf::Color(0xFFFF00FFU),
    sf::Color(0x333333FFU), sf::Color(0x0066FFFFU),
};

[[nodiscard]] engine::Substance Element::GetSubstance() const {
  return substance_;
}

[[nodiscard]] int8_t Element::GetSpeed() const {
  return horizontal_speed_;
}

void Element::SetSpeed(const int8_t speed) {
  horizontal_speed_ = speed;
}

float Element::GetVerticalSpeed() const {
  return vertical_speed_;
}

void Element::GravityAffect() {
  vertical_speed_ = std::min(kMaxVerticalSpeed, vertical_speed_ + constants::kGravityAcceleration);
}

void Element::StopFall() {
  //NOLINTNEXTLINE(readability-magic-numbers)
  vertical_speed_ = kDefaultVerticalSpeed;
}
