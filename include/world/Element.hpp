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

  [[nodiscard]] engine::Substance GetSubstance() const;

  [[nodiscard]] int8_t GetSpeed() const;

  void SetSpeed(int8_t speed);

  [[nodiscard]] sf::Color GetColor() const;

  [[nodiscard]] float GetVerticalSpeed() const;

  void GravityAffect();

  void StopFall();

 private:
  engine::Substance substance_;
  // uint8_t color_sample_;
  int8_t horizontal_speed_;
  float vertical_speed_;
};

#endif /* ELEMENT_HPP_ */
