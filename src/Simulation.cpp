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
	const Substance down_subs = get_substance_at(y + 1, x);

	if (SUBS_NOT_SOLID(down_subs) &&
		SUBS_IS_MOVABLE(down_subs))
	{
		swap_elements(y, x, y + 1, x, true);
		return true;
	}

	const int random_bit = terrain_rng.rand() & 1;
	const int first_x  = x + (random_bit ? 1 : -1);
	const int second_x = x + (random_bit ? -1 : 1);

	const Substance first_subs = get_substance_at(y + 1, first_x);
	const Substance second_subs = get_substance_at(y + 1, second_x);
	
	if (SUBS_IS_MOVABLE(first_subs) &&
		SUBS_NOT_SOLID(first_subs) &&
		SUBS_NOT_SOLID(get_substance_at(y, first_x)))
	{
		swap_elements(y, x, y + 1, first_x, true);
		return true;
	}
	if (SUBS_IS_MOVABLE(second_subs) &&
		SUBS_NOT_SOLID(second_subs) &&
		SUBS_NOT_SOLID(get_substance_at(y, second_x)))
	{
		swap_elements(y, x, y + 1, second_x, true);
		return true;
	}

	return false;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_STONE(const int y, const int x) {
	return false;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
bool FallingSandEngine::law_for_WATER(const int y, const int x) {
	const Substance down_subs = get_substance_at(y + 1, x);

	if (SUBS_NOT_SOLID(down_subs) &&
		SUBS_IS_MOVABLE(down_subs) &&
		SUBS_DENSITY(down_subs) < SUBS_DENSITY(Substance::WATER))
	{
		swap_elements(y, x, y + 1, x, true);
		return true;
	}

	const int direction = get_element_at(y, x).flow_direction;
	const int first_x  = x + direction;
	const int second_x = x - direction;

	const Substance first_subs = get_substance_at(y + 1, first_x);
	const Substance second_subs = get_substance_at(y + 1, second_x);
	
	const Substance up_first_subs = get_substance_at(y, first_x);
	const Substance up_second_subs = get_substance_at(y, second_x);
	
	if (SUBS_IS_MOVABLE(first_subs) &&
		SUBS_NOT_SOLID(first_subs) &&
		SUBS_NOT_SOLID(up_first_subs) &&
		SUBS_DENSITY(first_subs) < SUBS_DENSITY(Substance::WATER))
	{
		swap_elements(y, x, y + 1, first_x, true);
		return true;
	}
	if (SUBS_IS_MOVABLE(second_subs) &&
		SUBS_NOT_SOLID(second_subs) &&
		SUBS_NOT_SOLID(up_second_subs) &&
		SUBS_DENSITY(second_subs) < SUBS_DENSITY(Substance::WATER))
	{
		reverse_element_flow(y, x);
		swap_elements(y, x, y + 1, second_x, true);
		return true;
	}

	if (SUBS_IS_MOVABLE(up_first_subs) &&
		SUBS_NOT_SOLID(up_first_subs) &&
		SUBS_DENSITY(up_first_subs) < SUBS_DENSITY(Substance::WATER))
	{
		swap_elements(y, x, y, first_x, true);
		return true;
	}
	if (SUBS_IS_MOVABLE(up_second_subs) &&
		SUBS_NOT_SOLID(up_second_subs) &&
		SUBS_DENSITY(up_second_subs) < SUBS_DENSITY(Substance::WATER))
	{
		reverse_element_flow(y, x);
		swap_elements(y, x, y, second_x, true);
		return true;
	}

	return false;
}
