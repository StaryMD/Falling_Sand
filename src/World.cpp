#include "World.hpp"
#include "simulation.hpp"

#include <algorithm>

World::World() {
    this->grid = new Element[WORLD_HEIGHT * WORLD_WIDTH];
    std::fill(grid, grid + WORLD_HEIGHT * WORLD_WIDTH, Element(Substance::AIR));

	terrain_rng = FastRng();
}

void World::update() {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
			const Substance substance = get_element_at(y, x).substance;
			
            if (SUBS_IS_INVERSELY_UPDATED(get_element_at(y, x).substance) == false)
                laws[(int) substance](*this, y, x);
        }
    }

    for (int y = WORLD_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
			const Substance substance = get_element_at(y, x).substance;
			
            if (SUBS_IS_INVERSELY_UPDATED(get_element_at(y, x).substance) == true)
                laws[(int) substance](*this, y, x);
        }
    }
}

World::~World() {
    delete[] grid;
}
