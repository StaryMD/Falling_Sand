#ifndef COMMON_UTILITY_HPP_
#define COMMON_UTILITY_HPP_

#include <cmath>
#include <cstdint>
#include <string>

#include <SFML/System/Vector2.hpp>

/**
 * @brief Execute a function for every pixel on a line.
 * 
 * @details The type of the function is sent at compile time to vastly improve performance.
 * @tparam functor Type of the function to be executed.
 * @param start_point Start point of the line.
 * @param end_point End point of the line.
 * @param do_function The function to be executed.
 */
template <typename functor>
void ExecuteInALine(const sf::Vector2<int32_t> start_point, const sf::Vector2<int32_t> end_point,
                    const functor& do_function) {
  float move_x = static_cast<float>(end_point.x - start_point.x);
  float move_y = static_cast<float>(end_point.y - start_point.y);

  const int32_t step_count = static_cast<int32_t>(std::max(std::abs(move_x), std::abs(move_y)));

  move_x /= static_cast<float>(step_count);
  move_y /= static_cast<float>(step_count);

  float point_on_line_x = static_cast<float>(start_point.x);
  float point_on_line_y = static_cast<float>(start_point.y);

  for (int32_t i = 0; i <= step_count; i++) {
    do_function(point_on_line_x, point_on_line_y);
    point_on_line_x += move_x;
    point_on_line_y += move_y;
  }
}

/**
 * @brief Execute a function for every pixel on a circle.
 * 
 * @details The type of the function is sent at compile time to vastly improve performance.
 * The coordinates where the function is executed are relative to the coordinates of the function.
 * @tparam functor Type of the function to be executed.
 * @param start_point Radius of the circle.
 * @param do_function The function to be executed.
 */
template <typename functor>
void ExecuteInACircle(const int32_t radius, const functor& do_function) {
  if (radius == 0) {
    do_function(0, 0);
    return;
  }

  //NOLINTBEGIN(readability-identifier-length)
  int32_t t1 = static_cast<int32_t>(std::sqrt(radius));
  int32_t x = radius;
  int32_t y = 0;

  {
    {
      do_function(x, y);
      do_function(-x, y);

      do_function(y, x);
      do_function(y, -x);
    }

    ++y;
    t1 += y;
    const int32_t t2 = t1 - x;
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
    const int32_t t2 = t1 - x;
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

/**
 * @brief Execute a function for every pixel in a disc.
 * 
 * @details The type of the function is sent at compile time to vastly improve performance.
 * The coordinates where the function is executed are relative to the coordinates of the function.
 * @tparam functor Type of the function to be executed.
 * @param start_point Radius of the disc.
 * @param do_function The function to be executed.
 */
template <typename functor>
void ExecuteInADisc(const int32_t radius, const functor& do_function) {
  if (radius == 0) {
    do_function(0, 0);
    return;
  }

  //NOLINTBEGIN(readability-identifier-length)
  int32_t t1 = static_cast<int32_t>(std::sqrt(radius));
  int32_t x = radius;
  int32_t y = 0;

  while (x >= y) {
    {
      for (int32_t i = -x; i <= x; ++i) {
        do_function(i, y);

        if (y != 0) {
          do_function(i, -y);
        }
      }
    }

    ++y;
    t1 += y;
    const int32_t t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;

      for (int32_t i = -y + 1; i <= y - 1; ++i) {
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

template <typename t, typename d>
static inline sf::Vector2<t> ToVector2(const sf::Vector2<d> vector) {
  return {static_cast<t>(vector.x), static_cast<t>(vector.y)};
}

std::string ReadFileContent(std::string_view filename);

#endif  // COMMON_UTILITY_HPP_
