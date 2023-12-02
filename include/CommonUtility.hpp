#ifndef COMMON_UTILITY_HPP_
#define COMMON_UTILITY_HPP_

#include <functional>
#include <string>

#include <SFML/System/Vector2.hpp>

void ExecuteInALine(sf::Vector2i start_point, sf::Vector2i end_point,
                    const std::function<void(const float point_on_line_x, const float point_on_line_y)>& do_function);

void ExecuteInACircle(int radius,
                      const std::function<void(const int point_on_circle_x, const int point_on_circle_y)>& do_function);

template <typename t, typename d>
static inline sf::Vector2<t> ToVector2(const sf::Vector2<d> vector) {
  return {static_cast<t>(vector.x), static_cast<t>(vector.y)};
}

std::string ReadFileContent(const std::string& filename);

#endif /* COMMON_UTILITY_HPP_ */
