#pragma once

#include "Element.hpp"
#include "random_generators.hpp"

constexpr int WORLD_WIDTH = 1600;
constexpr int WORLD_HEIGHT = 800;

struct World {
    World();
    ~World();

    void update();

    Element get_element_at(const int y, const int x) const {
        return grid[y * WORLD_WIDTH + x];
    }

    void set_element_at(const int y, const int x, const Element subs) {
        grid[y * WORLD_WIDTH + x] = subs;
    }

    Element* grid;

    FastRng terrain_rng;

};
