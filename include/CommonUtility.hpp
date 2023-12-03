#ifndef COMMON_UTILITY_HPP_
#define COMMON_UTILITY_HPP_

#include <string>

#include <SFML/System/Vector2.hpp>

template <typename functor>
void ExecuteInALine(sf::Vector2i start_point, sf::Vector2i end_point, const functor& do_function);

template <typename functor>
void ExecuteInACircle(int radius, const functor& do_function);

template <typename functor>
void ExecuteInADisc(int radius, const functor& do_function);

template <typename t, typename d>
static inline sf::Vector2<t> ToVector2(const sf::Vector2<d> vector) {
  return {static_cast<t>(vector.x), static_cast<t>(vector.y)};
}

std::string ReadFileContent(std::string_view filename);

#endif /* COMMON_UTILITY_HPP_ */
