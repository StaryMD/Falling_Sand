#include "world/ElementProperty.hpp"

ElementProperty::ElementProperty(const uint8_t speed, const uint8_t unmovable)
    : direction_(speed), unmovable_(unmovable) {}

uint8_t ElementProperty::GetDirection() const {
  return direction_;
}

void ElementProperty::SetDirection(uint8_t speed) {
  direction_ = speed;
}

uint8_t ElementProperty::GetUnmovable() const {
  return unmovable_;
}

void ElementProperty::SetUnmovable(uint8_t unmovable) {
  unmovable_ = unmovable;
}
