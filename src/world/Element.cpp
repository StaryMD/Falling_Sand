#include <cstdint>
#include <sstream>

#include "world/Element.hpp"

Element::Element() : substance_(engine::Substance::kNothing) {
  property_.SetDirection(0);
}

Element::Element(const engine::Substance subs, const uint8_t current_update_tick_boolean) : substance_(subs) {
  property_.SetUnmovable(!current_update_tick_boolean);
}

const std::vector<sf::Color> kColors{
    sf::Color(0x000000FFU), sf::Color(0xD0D0D0FFU), sf::Color(0xFFFF00FFU),
    sf::Color(0x333333FFU), sf::Color(0x0066FFFFU),
};

sf::Color Element::GetColor() const {
  return kColors[static_cast<int>(GetSubstance())];
}

std::string Element::GetInformation() const {
  std::stringstream str_stream;

  switch (substance_) {
    case engine::Substance::kNothing: {
      str_stream << "Nothing\n";
      break;
    }
    case engine::Substance::kAir: {
      str_stream << "Air\n";
      break;
    }
    case engine::Substance::kSand: {
      str_stream << "Sand\n";
      break;
    }
    case engine::Substance::kStone: {
      str_stream << "Stone\n";
      break;
    }
    case engine::Substance::kWater: {
      const std::string direction = (static_cast<int>(property_.GetDirection())) ? "right" : "left";
      str_stream << "Water\n"
                 << "Direction: " << direction << '\n';
      break;
    }
    default: {
      return "very bad error\n";
    }
  }

  return str_stream.str();
}
