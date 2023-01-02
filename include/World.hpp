#pragma once

#include "Element.hpp"
#include "random_generators.hpp"

constexpr int WORLD_WIDTH = 1600;
constexpr int WORLD_HEIGHT = 800;

struct World {
    World();
    ~World();

    void update();

    void swap_elements(const int y1, const int x1, const int y2, const int x2) {
        const Element temp = grid[y1 * WORLD_WIDTH + x1];
        grid[y1 * WORLD_WIDTH + x1] = grid[y2 * WORLD_WIDTH + x2];
        grid[y2 * WORLD_WIDTH + x2] = temp;
    }

    Element get_element_at(const int y, const int x) const {
        return grid[y * WORLD_WIDTH + x];
    }

    void set_element_at(const int y, const int x, const Element subs) {
        grid[y * WORLD_WIDTH + x] = subs;
    }

    Element* grid;

    FastRng terrain_rng;

};
