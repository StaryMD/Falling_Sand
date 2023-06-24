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

 private:
  engine::Substance substance_;
};

#endif /* ELEMENT_HPP_ */
