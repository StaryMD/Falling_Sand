#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <string>

#include <SFML/Graphics/Color.hpp>

#include "world/ElementProperty.hpp"
#include "world/Substance.hpp"

class Element {
 public:
  Element();

  Element(engine::Substance subs, uint8_t current_update_tick_boolean);

  Element(const Element&) = default;
  Element(Element&&) = default;
  Element& operator=(const Element&) = default;
  Element& operator=(Element&&) = default;

  [[nodiscard]] engine::Substance GetSubstance() const { return substance_; }

  [[nodiscard]] uint8_t GetDirection() const { return property_.GetDirection(); }

  void SetDirection(const uint8_t speed) { property_.SetDirection(speed); }

  [[nodiscard]] sf::Color GetColor() const;

  [[nodiscard]] std::string GetInformation() const;

  [[nodiscard]] uint8_t GetUnmoved() const { return property_.GetUnmovable(); }

  void SetUnmoved(uint8_t value) { property_.SetUnmovable(value); }

 private:
  engine::Substance substance_;
  uint8_t color_sample_;
  ElementProperty property_;
};

#endif /* ELEMENT_HPP_ */
