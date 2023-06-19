#ifndef COMMON_UTILITY_HPP_
#define COMMON_UTILITY_HPP_

#include <functional>

#include <SFML/System/Vector2.hpp>

void ExecuteInALine(sf::Vector2i start_point, sf::Vector2i end_point,
                    const std::function<void(const float point_on_line_x, const float point_on_line_y)>& do_function);

sf::Vector2i ToVector2i(sf::Vector2f vector);

#endif /* COMMON_UTILITY_HPP_ */
