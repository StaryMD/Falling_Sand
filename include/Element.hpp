#pragma once

#include <SFML/Graphics.hpp>

enum class Substance {
    NOTHING = 0,
    AIR = 1,
    SAND = 2,
    STONE = 3,
};
constexpr int SUBSTANCE_COUNT = 4;

struct Element {
    Substance substance;

    Element();
    Element(const Substance substance);
};

static sf::Color color_of_substance[SUBSTANCE_COUNT] = {
    sf::Color::Red,
    sf::Color(101, 101, 101),
    sf::Color::Yellow,
    sf::Color(51, 51, 51),
};

static bool SUBS_IS_SOLID_vec[SUBSTANCE_COUNT] = {
    false,
    false,
    true,
    true
};

static inline bool SUBS_IS_SOLID(const Substance subs) {
    return SUBS_IS_SOLID_vec[(int) subs];
}
