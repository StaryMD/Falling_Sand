#pragma once

#include "random_generators.hpp"

constexpr int WORLD_WIDTH = 1600;
constexpr int WORLD_HEIGHT = 800;

constexpr int SUBSTANCE_COUNT = 3;
enum class Substance {
    NOTHING = 0,
    AIR = 1,
    SAND = 2,
};

struct World {

    World();
    ~World();

    void update();

    Substance get_subs_at(const int y, const int x) const {
        return grid[y * WORLD_WIDTH + x];
    }

    void set_subs_at(const int y, const int x, const Substance subs) {
        grid[y * WORLD_WIDTH + x] = subs;
    }

    void set_subs_at_temp(const int y, const int x, const Substance subs) {
        grid_temp[y * WORLD_WIDTH + x] = subs;
    }

    Substance* grid;
    Substance* grid_temp;

    FastRng terrain_rng;
};
