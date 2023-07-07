#include "world/Element.hpp"

Element::Element() : substance_(engine::Substance::kNothing) {
  speed_ = -1;
}

Element::Element(const engine::Substance subs) : substance_(subs) {}

const std::vector<sf::Color> kColors{
    sf::Color(0x000000FFU), sf::Color(0xD0D0D0FFU), sf::Color(0xFFFF00FFU),
    sf::Color(0x333333FFU), sf::Color(0x0066FFFFU),
};

sf::Color Element::GetColor() const {
  return kColors[static_cast<int>(GetSubstance())];
}
