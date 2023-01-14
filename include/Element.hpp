#pragma once

#include <SFML/Graphics.hpp>

enum class Substance {
    NOTHING = 0,
    AIR = 1,
    SAND = 2,
    STONE = 3,
    WATER = 4,
};
constexpr int SUBSTANCE_COUNT = 5;

struct Element {
    Substance substance;

    Element();
    Element(const Substance substance);
};


// PROPERITIES

static const sf::Color color_of_substance[SUBSTANCE_COUNT] = {
    sf::Color::Red,
    sf::Color(101, 101, 101),
    sf::Color::Yellow,
    sf::Color(51, 51, 51),
    sf::Color(3, 119, 252),
};

static const bool SUBS_IS_SOLID_vec[SUBSTANCE_COUNT] = {
    false,
    false,
    true,
    true,
    false,
};

static const bool SUBS_IS_INVERSELY_UPDATED_vec[SUBSTANCE_COUNT] = {
    false,
    false,
    true,
    false,
    true,
};

static const bool SUBS_IS_MOVABLE_vec[SUBSTANCE_COUNT] = {
    false,
    true,
    false,
    true,
    true,
};

static inline bool SUBS_IS_SOLID(const Substance subs) {
    return SUBS_IS_SOLID_vec[(int) subs];
}

static inline bool SUBS_IS_INVERSELY_UPDATED(const Substance subs) {
    return SUBS_IS_INVERSELY_UPDATED_vec[(int) subs];
}

static inline bool SUBS_IS_MOVABLE(const Substance subs) {
    return SUBS_IS_MOVABLE_vec[(int) subs];
}
