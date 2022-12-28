#include "world.hpp"
#include "simulation.hpp"

#include <algorithm>

World::World() {
    this->grid      = new Substance[WORLD_HEIGHT * WORLD_WIDTH];
    this->grid_temp = new Substance[WORLD_HEIGHT * WORLD_WIDTH];

    std::fill(grid, grid + WORLD_HEIGHT * WORLD_WIDTH, Substance::AIR);

	terrain_rng = FastRng();
	init_laws_table();
}

void World::update() {
	//std::copy(grid, grid + WORLD_HEIGHT * WORLD_WIDTH, grid_temp);

    for (int y = WORLD_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
			const int substance = (int) get_subs_at(y, x);
			
            laws[substance](*this, y, x);
        }
    }
    std::swap(grid, grid_temp);
}

void World::init_laws_table() {
	laws[(int) Substance::NOTHING] = law_for_NOTHING;
	laws[(int) Substance::AIR    ] = law_for_AIR;
	laws[(int) Substance::SAND   ] = law_for_SAND;
}

World::~World() {
    delete[] grid;
    delete[] grid_temp;
}
