#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <SFML/Graphics/Color.hpp>

#include "world/Substance.hpp"

class Element {
 public:
  Element() : substance_(Substance::kNothing) {}

  Element(const Element&) = default;
  Element(Element&&) = default;
  Element& operator=(const Element&) = default;
  Element& operator=(Element&&) = default;

  explicit Element(const Substance subs) : substance_(subs) {}

  [[nodiscard]] Substance GetSubstance() const { return substance_; }

 private:
  Substance substance_;
};

#endif /* ELEMENT_HPP_ */
