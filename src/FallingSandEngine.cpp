#include "FallingSandEngine.hpp"

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdio>

#include "CameraView.hpp"
#include "world/Substance.hpp"

FallingSandEngine::FallingSandEngine(const sf::Vector2i size) : world_(size) {}

FallingSandEngine::FallingSandEngine(const sf::Vector2u size) : world_(size) {}

void FallingSandEngine::PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes,
                                sf::Vector2u screen_size) const {
  const std::vector<sf::Color> rand_colors = {
      sf::Color(255, 255, 255), sf::Color(200, 200, 200), sf::Color(180, 180, 180), sf::Color(150, 150, 150),
      sf::Color(100, 100, 100), sf::Color(50, 50, 50),    sf::Color(0, 0, 0),
  };

  sf::Color* pixels = reinterpret_cast<sf::Color*>(bytes.data());
  sf::Color* pixel_it = pixels;

  const sf::Rect<double> view = camera_view.GetFieldOfView();
  const double step_x = static_cast<double>(view.width) / screen_size.x;
  const double step_y = static_cast<double>(view.height) / screen_size.y;

  for (unsigned coord_y = 0; coord_y < screen_size.y; ++coord_y) {
    for (unsigned coord_x = 0; coord_x < screen_size.x; ++coord_x) {
      const int world_coord_x = static_cast<int>(view.left + coord_x * step_x);
      const int world_coord_y = static_cast<int>(view.top + coord_y * step_y);

      *(pixel_it++) = world_.GetColorAt({world_coord_x, world_coord_y});
    }
  }
}
