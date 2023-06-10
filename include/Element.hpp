#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include "Substance.hpp"

class Element {
 public:
  Element() : substance(Substance::kSand) {}

  Substance substance;
};

#endif /* ELEMENT_HPP_ */
