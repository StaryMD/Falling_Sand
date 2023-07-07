#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <SFML/Graphics/Color.hpp>

#include "world/Substance.hpp"

class Element {
 public:
  Element();

  explicit Element(engine::Substance subs);

  Element(const Element&) = default;
  Element(Element&&) = default;
  Element& operator=(const Element&) = default;
  Element& operator=(Element&&) = default;

  [[nodiscard]] engine::Substance GetSubstance() const { return substance_; }

  [[nodiscard]] int8_t GetSpeed() const { return speed_; }

  void SetSpeed(const int8_t speed) { speed_ = speed; }

  [[nodiscard]] sf::Color GetColor() const;

 private:
  engine::Substance substance_;
  uint8_t color_sample_;
  int8_t speed_;
};

#endif /* ELEMENT_HPP_ */
