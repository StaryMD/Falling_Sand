#include "CommonUtility.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include <SFML/System/Vector2.hpp>

void ExecuteInALine(const sf::Vector2i start_point, const sf::Vector2i end_point,
                    const std::function<void(const float point_on_line_x, const float point_on_line_y)>& do_function) {
  float move_x = static_cast<float>(end_point.x - start_point.x);
  float move_y = static_cast<float>(end_point.y - start_point.y);

  const int step_count = static_cast<int>(std::max(std::abs(move_x), std::abs(move_y)));

  move_x /= static_cast<float>(step_count);
  move_y /= static_cast<float>(step_count);

  float point_on_line_x = static_cast<float>(start_point.x);
  float point_on_line_y = static_cast<float>(start_point.y);

  for (int i = 0; i <= step_count; i++) {
    do_function(point_on_line_x, point_on_line_y);
    point_on_line_x += move_x;
    point_on_line_y += move_y;
  }
}

void ExecuteInACircle(
    const int radius,
    const std::function<void(const int point_on_circle_x, const int point_on_circle_y)>& do_function) {
  if (radius == 0) {
    do_function(0, 0);
    return;
  }

  //NOLINTBEGIN(readability-identifier-length)
  int t1 = static_cast<int>(std::sqrt(radius));
  int x = radius;
  int y = 0;

  {
    {
      do_function(x, y);
      do_function(-x, y);

      do_function(y, x);
      do_function(y, -x);
    }

    ++y;
    t1 += y;
    const int t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;
      --x;
    }
  }

  while (x > y) {
    {
      do_function(x, y);
      do_function(-x, y);

      do_function(x, -y);
      do_function(-x, -y);

      do_function(y, x);
      do_function(y, -x);

      do_function(-y, x);
      do_function(-y, -x);
    }

    ++y;
    t1 += y;
    const int t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;
      --x;
    }
  }

  if (x == y) {
    do_function(x, y);
    do_function(-x, y);

    do_function(x, -y);
    do_function(-x, -y);
  }
  //NOLINTEND(readability-identifier-length)
}

void ExecuteInADisc(const int radius,
                    const std::function<void(const int point_on_disc_x, const int point_on_disc_y)>& do_function) {
  if (radius == 0) {
    do_function(0, 0);
    return;
  }

  //NOLINTBEGIN(readability-identifier-length)
  int t1 = static_cast<int>(std::sqrt(radius));
  int x = radius;
  int y = 0;

  while (x >= y) {
    {
      for (int i = -x; i <= x; ++i) {
        do_function(i, y);

        if (y != 0) {
          do_function(i, -y);
        }
      }
    }

    ++y;
    t1 += y;
    const int t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;

      for (int i = -y + 1; i <= y - 1; ++i) {
        if (x != (y - 1)) {
          do_function(i, x);
          do_function(i, -x);
        }
      }

      --x;
    }
  }
  //NOLINTEND(readability-identifier-length)
}

std::string ReadFileContent(const std::string& filename) {
  const std::ifstream file(filename);
  std::stringstream str_stream;
  str_stream << file.rdbuf();
  return str_stream.str();
}
