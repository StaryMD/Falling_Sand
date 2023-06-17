#include "CommonUtility.hpp"

#include <cmath>
#include <cstdio>

void ExecuteInALine(const sf::Vector2i start_point, const sf::Vector2i end_point,
                    const std::function<void(const float point_on_line_x, const float point_on_line_y)>& do_function) {
  float move_x = static_cast<float>(end_point.x - start_point.x);
  float move_y = static_cast<float>(end_point.y - start_point.y);

  const int step_count = static_cast<int>(std::max(std::abs(move_x), std::abs(move_y)));

  move_x /= static_cast<float>(step_count);
  move_y /= static_cast<float>(step_count);

  float point_on_line_x = static_cast<float>(start_point.x);
  float point_on_line_y = static_cast<float>(start_point.y);

  for (int i = 0; i < step_count; i++) {
    do_function(point_on_line_x, point_on_line_y);
    point_on_line_x += move_x;
    point_on_line_y += move_y;
  }
}
