#include "world.hpp"

#include <algorithm>

#pragma GCC diagnostic ignored "-Wunused-parameter"
void (*laws[SUBSTANCE_COUNT])(World& world, const int y, const int x) = {
	[] (World& world, const int y, const int x) { // NOTHING
	},
	[] (World& world, const int y, const int x) { // AIR
		world.set_subs_at_temp(y, x, Substance::AIR);
	},
	[] (World& world, const int y, const int x) { // SAND
		const int wanted_y = y + 1;

		if (wanted_y < WORLD_HEIGHT) {
			if (world.get_subs_at(wanted_y, x) != Substance::SAND) {
				world.set_subs_at_temp(y, x, Substance::AIR);
				world.set_subs_at_temp(wanted_y, x, Substance::SAND);
				world.set_subs_at(y, x, Substance::NOTHING);
				return;
			}

			int first_x, second_x;
			
			if (world.terrain_rng.rand() & 1) {
				first_x = x - 1;
				second_x = x + 1;
			}
			else {
				first_x = x + 1;
				second_x = x - 1;
			}
			
			if (first_x < WORLD_WIDTH && world.get_subs_at(wanted_y, first_x) != Substance::SAND) {
				world.set_subs_at_temp(y, x, Substance::AIR);
				world.set_subs_at_temp(wanted_y, first_x, Substance::SAND);
				world.set_subs_at(y, x, Substance::NOTHING);
				return;
			}
			if (second_x >= 0 && world.get_subs_at(wanted_y, second_x) != Substance::SAND) {
				world.set_subs_at_temp(y, x, Substance::AIR);
				world.set_subs_at_temp(wanted_y, second_x, Substance::SAND);
				world.set_subs_at(y, x, Substance::NOTHING);
				return;
			}
		}

		world.set_subs_at_temp(y, x, Substance::SAND);
	},
};

World::World() {
    this->grid      = new Substance[WORLD_HEIGHT * WORLD_WIDTH];
    this->grid_temp = new Substance[WORLD_HEIGHT * WORLD_WIDTH];

    std::fill(grid, grid + WORLD_HEIGHT * WORLD_WIDTH, Substance::AIR);

	terrain_rng = FastRng();
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

World::~World() {
    delete[] grid;
    delete[] grid_temp;
}
