#include "FallingSandEngine.hpp"

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_NOTHING(const int y, const int x) {
	return false;
	// nothing, wow
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_AIR(const int y, const int x) {
	return false;
}

bool FallingSandEngine::law_for_SAND(const int y, const int x) {
	if (y == WORLD_HEIGHT - 1) {
		set_cell(y, x, Element(Substance::AIR), true);
		return true;
	} else {
		if (false == SUBS_IS_SOLID(get_element_at(y + 1, x).substance)) {
			swap_elements(y, x, y + 1, x, true);
			return true;
		}

		const int random_bit = terrain_rng.rand() & 1;
		const int first_x  = x - random_bit;
		const int second_x = x + random_bit;

		const Substance first_subs = get_element_at(y + 1, first_x).substance;
		const Substance second_subs = get_element_at(y + 1, second_x).substance;
		
		if (first_x < WORLD_WIDTH && SUBS_IS_MOVABLE(first_subs) &&
			false == SUBS_IS_SOLID(get_element_at(y + 1, first_x).substance) &&
			false == SUBS_IS_SOLID(get_element_at(y    , first_x).substance))
		{
			swap_elements(y, x, y + 1, first_x, true);
			return true;
		}
		if (second_x >= 0 && SUBS_IS_MOVABLE(second_subs) &&
			false == SUBS_IS_SOLID(get_element_at(y + 1, second_x).substance) &&
			false == SUBS_IS_SOLID(get_element_at(y    , second_x).substance))
		{
			swap_elements(y, x, y + 1, second_x, true);
			return true;
		}
	}

	return false;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_STONE(const int y, const int x) {
	return false;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_WATER(const int y, const int x) {
	return false;
}
