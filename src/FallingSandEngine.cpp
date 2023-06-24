#include "FallingSandEngine.hpp"

#include <cstdio>

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "CommonUtility.hpp"
#include "world/Element.hpp"
#include "world/Substance.hpp"

FallingSandEngine::FallingSandEngine(const sf::Vector2i size) : world_(size) {}

FallingSandEngine::FallingSandEngine(const sf::Vector2u size) : world_(size) {}

void FallingSandEngine::PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes,
                                sf::Vector2u screen_size) const {
  sf::Color* pixel_it = reinterpret_cast<sf::Color*>(bytes.data());

  const sf::Rect<double> view = camera_view.GetFieldOfView();
  const double step_x = static_cast<double>(view.width) / screen_size.x;
  const double step_y = static_cast<double>(view.height) / screen_size.y;

  double world_coord_y = view.top;

  for (unsigned screen_y = 0; screen_y < screen_size.y; ++screen_y, world_coord_y += step_y) {
    double world_coord_x = view.left;

    for (unsigned screen_x = 0; screen_x < screen_size.x; ++screen_x, world_coord_x += step_x) {
      *(pixel_it++) = world_.GetColorAt({static_cast<int>(world_coord_x), static_cast<int>(world_coord_y)});
    }
  }
}

void FallingSandEngine::PlaceElementInLine(const sf::Vector2i start_pos, const sf::Vector2i end_pos,
                                           const engine::Substance substance) {
  ExecuteInALine(start_pos, end_pos, [&](const float point_on_line_x, const float point_on_line_y) {
    world_.SetElementAt(ToVector2<int, float>({point_on_line_x, point_on_line_y}), Element(substance));
  });
}
