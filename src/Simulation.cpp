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
		const int first_x  = x + (random_bit ? 1 : -1);
		const int second_x = x + (random_bit ? -1 : 1);

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
	if (y == WORLD_HEIGHT - 1) {
		set_cell(y, x, Element(Substance::AIR), true);
		return true;
	} else {
		const Substance down_subs = get_element_at(y + 1, x).substance;

		if (false == SUBS_IS_SOLID(down_subs) &&
			SUBS_DENSITY_vec[(int) down_subs] < SUBS_DENSITY_vec[(int) Substance::WATER])
		{
			swap_elements(y, x, y + 1, x, true);
			return true;
		}

		const int direction = get_element_at(y, x).flow_direction;
		const int first_x  = x + (direction ? 1 : -1);
		const int second_x = x + (direction ? -1 : 1);

		const Substance first_subs = get_element_at(y + 1, first_x).substance;
		const Substance second_subs = get_element_at(y + 1, second_x).substance;
		
		const Substance up_first_subs = get_element_at(y, first_x).substance;
		const Substance up_second_subs = get_element_at(y, second_x).substance;
		
		if (first_x < WORLD_WIDTH && SUBS_IS_MOVABLE(first_subs) &&
			false == SUBS_IS_SOLID(first_subs) &&
			false == SUBS_IS_SOLID(up_first_subs) &&
			SUBS_DENSITY_vec[(int) first_subs] < SUBS_DENSITY_vec[(int) Substance::WATER])
		{
			swap_elements(y, x, y + 1, first_x, true);
			return true;
		}
		if (second_x >= 0 && SUBS_IS_MOVABLE(second_subs) &&
			false == SUBS_IS_SOLID(second_subs) &&
			false == SUBS_IS_SOLID(up_second_subs) &&
			SUBS_DENSITY_vec[(int) second_subs] < SUBS_DENSITY_vec[(int) Substance::WATER])
		{
			reverse_element_flow(y, x);
			swap_elements(y, x, y + 1, second_x, true);
			return true;
		}

		if (first_x < WORLD_WIDTH && SUBS_IS_MOVABLE(up_first_subs) &&
			false == SUBS_IS_SOLID(up_first_subs) &&
			SUBS_DENSITY_vec[(int) up_first_subs] < SUBS_DENSITY_vec[(int) Substance::WATER])
		{
			swap_elements(y, x, y, first_x, true);
			return true;
		}
		if (second_x >= 0 && SUBS_IS_MOVABLE(up_second_subs) &&
			false == SUBS_IS_SOLID(up_second_subs) &&
			SUBS_DENSITY_vec[(int) up_second_subs] < SUBS_DENSITY_vec[(int) Substance::WATER])
		{
			reverse_element_flow(y, x);
			swap_elements(y, x, y, second_x, true);
			return true;
		}

	}

	return false;
}
