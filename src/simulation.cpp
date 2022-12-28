#include "simulation.hpp"
#include "world.hpp"

#include <algorithm>

#pragma GCC diagnostic ignored "-Wunused-parameter"
void law_for_NOTHING(World& world, const int y, const int x) {
	// nothing, wow
}

void law_for_AIR(World& world, const int y, const int x) {
	world.set_subs_at_temp(y, x, Substance::AIR);
}

void law_for_SAND(World& world, const int y, const int x) {
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
}
