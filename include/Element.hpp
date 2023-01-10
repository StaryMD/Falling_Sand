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


// PROPERITIES

static const sf::Color color_of_substance[SUBSTANCE_COUNT] = {
    sf::Color::Red,
    sf::Color(101, 101, 101),
    sf::Color::Yellow,
    sf::Color(51, 51, 51),
};

static const bool SUBS_IS_SOLID_vec[SUBSTANCE_COUNT] = {
    false,
    false,
    true,
    true
};

static inline bool SUBS_IS_SOLID(const Substance subs) {
    return SUBS_IS_SOLID_vec[(int) subs];
}

static const bool SUBS_IS_INVERSELY_UPDATED_vec[SUBSTANCE_COUNT] = {
    false,
    false,
    true,
    false
};

static inline bool SUBS_IS_INVERSELY_UPDATED(const Substance subs) {
    return SUBS_IS_INVERSELY_UPDATED_vec[(int) subs];
}

static const bool SUBS_IS_MOVABLE_vec[SUBSTANCE_COUNT] = {
    false,
    true,
    false,
    true
};

static inline bool SUBS_IS_MOVABLE(const Substance subs) {
    return SUBS_IS_MOVABLE_vec[(int) subs];
}

static inline bool SUBS_IS_MOVABLE_AND_SOLID(const Substance subs) {
    return SUBS_IS_MOVABLE_vec[(int) subs] && SUBS_IS_SOLID_vec[(int) subs];
}
