#ifndef ELEMENT_PROPERTY_HPP_
#define ELEMENT_PROPERTY_HPP_

#include <string>

#include <SFML/Graphics/Color.hpp>

#include "world/Substance.hpp"

class ElementProperty {
 public:
  explicit ElementProperty(uint8_t speed = 0, uint8_t unmovable = 0);

  [[nodiscard]] uint8_t GetDirection() const;
  void SetDirection(uint8_t speed);

  [[nodiscard]] uint8_t GetUnmovable() const;
  void SetUnmovable(uint8_t unmovable);

 private:
  uint8_t direction_ : 1;
  uint8_t unmovable_ : 1;
};

#endif /* ELEMENT_PROPERTY_HPP_ */
