#include "Chunk.hpp"

#include "Element.hpp"

Chunk::Chunk() {
}

Chunk::Chunk(const int y, const int x) {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			elements[i][j] = Element(Substance::AIR);
		}
	}

	position = sf::Vector2i(y, x);
	is_active = true;
}
