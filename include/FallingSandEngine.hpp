#ifndef FALLING_SAND_ENGINE_HPP_
#define FALLING_SAND_ENGINE_HPP_

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

class FallingSandEngine {
 public:
  FallingSandEngine() = delete;

  explicit FallingSandEngine(sf::Vector2i size);

  explicit FallingSandEngine(sf::Vector2u size);

  void PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes, sf::Vector2u screen_size) const;

  void PlaceElementInLine(sf::Vector2i start_pos, sf::Vector2i end_pos, engine::Substance substance);

 private:
  World world_;
};

#endif /* FALLING_SAND_ENGINE_HPP_ */
