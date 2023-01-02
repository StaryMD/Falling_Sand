#pragma once

#include <SFML/Graphics.hpp>

#include "Element.hpp"
#include "Chunk.hpp"
#include "random_generators.hpp"
#include "constants.hpp"

class FallingSandEngine {
public:

	FallingSandEngine();

	void update();

	void draw_world_on_texture(sf::Uint8* screen_pixels);

	void set_cell(const int y, const int x, const Element &element, bool activate_chunk);

    Element get_element_at(const int y, const int x) const;
    void set_element_at(const int y, const int x, const Element &element, bool activate_chunk);

    void swap_elements(const int y1, const int x1, const int y2, const int x2, bool activate_chunk);
	
	void apply_law(const Substance substance, const int y, const int x);
	
	~FallingSandEngine();

private:
    Chunk** chunks;

    FastRng terrain_rng;
	
	void law_for_NOTHING(const int y, const int x);
	void law_for_AIR(const int y, const int x);
	void law_for_SAND(const int y, const int x);
	void law_for_STONE(const int y, const int x);

};
