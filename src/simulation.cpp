#include "simulation.hpp"
#include "World.hpp"

#include <algorithm>

#pragma GCC diagnostic ignored "-Wunused-parameter"
void law_for_NOTHING(World& world, const int y, const int x) {
	// nothing, wow
}

void law_for_AIR(World& world, const int y, const int x) {
	//world.set_element_at(y, x, Element(Substance::AIR));
}

void law_for_SAND(World& world, const int y, const int x) {
	if (y + 1 < WORLD_HEIGHT) {
		if (false == SUBS_IS_SOLID(world.get_element_at(y + 1, x).substance)) {
			world.set_element_at(y, x, Element(Substance::AIR));
			world.set_element_at(y + 1, x, Element(Substance::SAND));
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
		
		if (first_x < WORLD_WIDTH &&
			false == SUBS_IS_SOLID(world.get_element_at(y + 1, first_x).substance) &&
			false == SUBS_IS_SOLID(world.get_element_at(y    , first_x).substance))
		{
			world.set_element_at(y, x, Element(Substance::AIR));
			world.set_element_at(y + 1, first_x, Element(Substance::SAND));
			return;
		}
		if (second_x >= 0 &&
			false == SUBS_IS_SOLID(world.get_element_at(y + 1, second_x).substance) &&
			false == SUBS_IS_SOLID(world.get_element_at(y    , second_x).substance))
		{
			world.set_element_at(y, x, Element(Substance::AIR));
			world.set_element_at(y + 1, second_x, Element(Substance::SAND));
			return;
		}
	}

	world.set_element_at(y, x, Element(Substance::SAND));
}

void law_for_STONE(World& world, const int y, const int x) {
	//world.set_element_at(y, x, Element(Substance::STONE));
}
