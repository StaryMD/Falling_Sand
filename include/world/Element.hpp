#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <SFML/Graphics/Color.hpp>

#include "world/Substance.hpp"

class Element {
 public:
  Element() : substance_(engine::Substance::kNothing) {}

  Element(const Element&) = default;
  Element(Element&&) = default;
  Element& operator=(const Element&) = default;
  Element& operator=(Element&&) = default;

  explicit Element(const engine::Substance subs) : substance_(subs) {}

  [[nodiscard]] engine::Substance GetSubstance() const { return substance_; }

  [[nodiscard]] sf::Color GetColor() const;

 private:
  engine::Substance substance_;
  uint8_t color_sample_;
  int8_t speed_;
};

#endif /* ELEMENT_HPP_ */
