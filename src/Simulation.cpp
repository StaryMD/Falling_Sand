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
	if (y + 1 < WORLD_HEIGHT) {
		if (false == SUBS_IS_SOLID(get_element_at(y + 1, x).substance)) {
			swap_elements(y, x, y + 1, x, true);
			return true;
		}

		int first_x = x + 1;
		int second_x = x - 1;
		
		if (terrain_rng.rand() & 1) {
			first_x = x - 1;
			second_x = x + 1;
		}
		
		if (first_x < WORLD_WIDTH &&
			false == SUBS_IS_SOLID(get_element_at(y + 1, first_x).substance) &&
			false == SUBS_IS_SOLID(get_element_at(y    , first_x).substance))
		{
			swap_elements(y, x, y + 1, first_x, true);
			return true;
		}
		if (second_x >= 0 &&
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
