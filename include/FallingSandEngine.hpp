#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Element.hpp"
#include "Chunk.hpp"
#include "random_generators.hpp"
#include "constants.hpp"

class FallingSandEngine {
public:

	FallingSandEngine();

	void update();

	void draw_world_on_texture(sf::Uint8* screen_pixels);

	void set_cell(const int y, const int x, const Element &element, const bool activate_chunk);

    Substance get_substance_at(const int y, const int x);
	
    Element get_element_at(const int y, const int x);
    void set_element_at(const int y, const int x, const Element &element, const bool activate_chunk);

    void swap_elements(const int y1, const int x1, const int y2, const int x2, bool activate_chunk);
	
	bool apply_law(const Substance substance, const int y, const int x);

	void set_chunk_activity(const int chunk_y, const int chunk_x, const bool activity);

	void reverse_element_flow(const int y, const int x);

	void update_future_update_scheme(const int chunk_y, const int chunk_x, const bool activity);
	
	~FallingSandEngine();

private:
	using Chunks_2D_t = Chunk**;

    Chunks_2D_t chunks;

	std::vector<bool> future_update_scheme;

    FastRng terrain_rng;
	
	bool law_for_NOTHING(const int y, const int x);
	bool law_for_AIR(const int y, const int x);
	bool law_for_SAND(const int y, const int x);
	bool law_for_STONE(const int y, const int x);
	bool law_for_WATER(const int y, const int x);

};
