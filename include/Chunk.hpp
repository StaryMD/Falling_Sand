#pragma once

#include <SFML/Graphics.hpp>

#include "Element.hpp"
#include "constants.hpp"

struct Chunk {
    Element elements[CHUNK_SIZE][CHUNK_SIZE];
	sf::Vector2i position;
	bool is_active;

	Chunk();
	Chunk(const int y, const int x);

	Element get_element_at(const int y, const int x) const {
		return elements[y][x];
	}

};
