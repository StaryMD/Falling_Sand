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

    void set_element_at_temp(const int y, const int x, const Element subs) {
        grid_temp[y * WORLD_WIDTH + x] = subs;
    }

    void init_laws_table();

    Element* grid;
    Element* grid_temp;

    void (*laws[SUBSTANCE_COUNT])(World& world, const int y, const int x);

    FastRng terrain_rng;

};
