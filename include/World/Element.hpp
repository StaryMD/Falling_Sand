#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_

#include <SFML/Graphics/Color.hpp>

#include "World/Substance.hpp"

class Element {
 public:
  /**
   * @brief Construct a empty Element.
   * 
   */
  Element();

  /**
   * @brief Construct a new Element from a certain substance.
   * 
   * @param subs Substance basing the element.
   */
  explicit Element(engine::Substance subs);

  /**
   * @brief Copy constructor.
   * 
   */
  Element(const Element&) = default;

  /**
   * @brief Move constructor.
   * 
   */
  Element(Element&&) = default;

  /**
   * @brief Copy operator.
   * 
   * @return Element& Element to be copied from.
   */
  Element& operator=(const Element&) = default;

  /**
   * @brief Move operator.
   * 
   * @return Element& Element to be moved from.
   */
  Element& operator=(Element&&) = default;

  /**
   * @brief Get the substance of the Element.
   * 
   * @return engine::Substance Value of the underlying substance.
   */
  [[nodiscard]] engine::Substance GetSubstance() const;

  /**
   * @brief Get the current horizontal speed of the element.
   * 
   * @return int8_t Value of the horizontal speed component.
   */
  [[nodiscard]] int8_t GetHorizontalSpeed() const;

  /**
   * @brief Set the horizontal speed component to a new value.
   * 
   * @param speed New value for the horizontal speed component.
   */
  void SetHorizontalSpeed(int8_t speed);

  /**
   * @brief Get the current vertical speed of the element.
   * 
   * @return int8_t Value of the vertical speed component.
   */
  [[nodiscard]] float GetVerticalSpeed() const;

  /**
   * @brief Get the draw property of the elment.
   * 
   * @details The offset inside the texture used when drawing.
   * @return uint8_t Value of the draw property.
   */
  [[nodiscard]] uint8_t GetDrawProperty() const;

  /**
   * @brief Calculate a new value for the vertical speed, taking into account gravity.
   * 
   */
  void GravityAffect();

  /**
   * @brief Set the vertical speed to zero.
   * 
   */
  void StopFall();

  /**
   * @brief Check whether the element has non-zero vertical speed.
   * 
   * @return bool Whether the element has non-zero vertical speed.
   */
  [[nodiscard]] bool HasSignificantVerticalSpeed() const;

 private:
  engine::Substance substance_;
  uint8_t draw_property_;
  int8_t horizontal_speed_;
  float vertical_speed_;
};

#endif // ELEMENT_HPP_
