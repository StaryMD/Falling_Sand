#include "world/Element.hpp"

const std::vector<sf::Color> kColors{
    sf::Color::Black, sf::Color(200, 200, 200), sf::Color::Yellow, sf::Color(51, 51, 51), sf::Color::Blue,
};

sf::Color Element::GetColor() const {
  return kColors[static_cast<int>(GetSubstance())];
}
