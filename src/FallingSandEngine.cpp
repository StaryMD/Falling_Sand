#include "FallingSandEngine.hpp"

FallingSandEngine::FallingSandEngine() {
	chunks = new Chunk*[CHUNK_NUM_HEIGHT];

    for (int y = 0; y < CHUNK_NUM_HEIGHT; y++) {
		chunks[y] = new Chunk[CHUNK_NUM_WIDTH];

        for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
            chunks[y][x] = Chunk(y, x);
        }
    }
}

bool FallingSandEngine::apply_law(const Substance substance, const int y, const int x) {
	switch (substance) {
		case Substance::NOTHING: {
			return law_for_NOTHING(y, x);
		}
		case Substance::AIR: {
			return law_for_AIR(y, x);
		}
		case Substance::SAND: {
			return law_for_SAND(y, x);
		}
		case Substance::STONE: {
			return law_for_STONE(y, x);
		}
		case Substance::WATER: {
			return law_for_WATER(y, x);
		}
	}

	return false;
}

void FallingSandEngine::set_chunk_activity(const int chunk_y, const int chunk_x, const bool activity) {
	if (chunk_y >= 0 && chunk_y < CHUNK_NUM_HEIGHT && chunk_x >= 0 && chunk_x < CHUNK_NUM_WIDTH) {
		chunks[chunk_y][chunk_x].is_active = activity;
	}
}

void FallingSandEngine::reverse_element_flow(const int y, const int x) {
	if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT) {
		return;
	}

	const int chunk_y = y / CHUNK_SIZE, chunk_x = x / CHUNK_SIZE;
	Chunk& chunk = chunks[chunk_y][chunk_x];
	
	const int inside_chunk_y = y % CHUNK_SIZE, inside_chunk_x = x % CHUNK_SIZE;

	chunk.elements[inside_chunk_y][inside_chunk_x].reverse_flow();
}

void FallingSandEngine::update() {
    for (int y = 0; y < CHUNK_NUM_HEIGHT; y++) {
        for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
			Chunk &chunk = chunks[y][x];

            if (chunk.is_active) {
				bool there_is_more = false;
				bool was_activated = false;
				
				for (int i = 0; i < CHUNK_SIZE; i++) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						const Substance substance = chunk.get_element_at(i, j).substance;
						
						if (SUBS_IS_INVERSELY_UPDATED(substance) == false) {
							was_activated |= apply_law(substance, y * CHUNK_SIZE + i, x * CHUNK_SIZE + j);
						}
						else {
							there_is_more = true;
						}
					}
				}

				chunk.is_active = there_is_more;
				if (was_activated) {
					set_chunk_activity(y - 1, x, true);
					set_chunk_activity(y - 1, x + 1, true);
					set_chunk_activity(y, x + 1, true);
					set_chunk_activity(y + 1, x + 1, true);
					set_chunk_activity(y + 1, x, true);
					set_chunk_activity(y + 1, x - 1, true);
					set_chunk_activity(y, x - 1, true);
					set_chunk_activity(y - 1, x - 1, true);
				}
			}
        }
    }

    for (int y = CHUNK_NUM_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
			Chunk &chunk = chunks[y][x];

            if (chunk.is_active) {
				bool was_activated = false;

				for (int i = CHUNK_SIZE - 1; i >= 0; i--) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						const Substance substance = chunk.get_element_at(i, j).substance;
						
						if (SUBS_IS_INVERSELY_UPDATED(substance) == true) {
							was_activated |= apply_law(substance, y * CHUNK_SIZE + i, x * CHUNK_SIZE + j);
						}
					}
				}
				
				chunk.is_active = was_activated;
				if (was_activated) {
					set_chunk_activity(y - 1, x, true);
					set_chunk_activity(y - 1, x + 1, true);
					set_chunk_activity(y, x + 1, true);
					set_chunk_activity(y + 1, x + 1, true);
					set_chunk_activity(y + 1, x, true);
					set_chunk_activity(y + 1, x - 1, true);
					set_chunk_activity(y, x - 1, true);
					set_chunk_activity(y - 1, x - 1, true);
				}
			}
        }
    }
}

void FallingSandEngine::set_cell(const int y, const int x, const Element &element, const bool activate_chunk) {
	set_element_at(y, x, element, activate_chunk);
}

void FallingSandEngine::swap_elements(const int y1, const int x1, const int y2, const int x2, bool activate_chunk) {
	if (x1 < 0 || x1 >= WORLD_WIDTH || y1 < 0 || y1 >= WORLD_HEIGHT) {
		// First point is outside
		if (x2 < 0 || x2 >= WORLD_WIDTH || y2 < 0 || y2 >= WORLD_HEIGHT) {
			// Second point is outside

			// do nothing
		} else {
			// Second point is NOT outside

			const int chunk2_y = y2 / CHUNK_SIZE, chunk2_x = x2 / CHUNK_SIZE;
			const int inside_chunk2_y = y2 % CHUNK_SIZE, inside_chunk2_x = x2 % CHUNK_SIZE;
			Chunk& chunk2 = chunks[chunk2_y][chunk2_x];
			chunk2.elements[inside_chunk2_y][inside_chunk2_x] = Element(Substance::AIR);
			chunk2.is_active |= activate_chunk;
		}

		return;
	} else {
		// First point is NOT outside
		if (x2 < 0 || x2 >= WORLD_WIDTH || y2 < 0 || y2 >= WORLD_HEIGHT) {
			// Second point is outside
			const int chunk1_y = y1 / CHUNK_SIZE, chunk1_x = x1 / CHUNK_SIZE;
			const int inside_chunk1_y = y1 % CHUNK_SIZE, inside_chunk1_x = x1 % CHUNK_SIZE;
			Chunk& chunk1 = chunks[chunk1_y][chunk1_x];
			chunk1.elements[inside_chunk1_y][inside_chunk1_x] = Element(Substance::AIR);
			chunk1.is_active |= activate_chunk;
			return;
		} else {
			// Second point is NOT outside

			// do nothing
		}
	}

	const int chunk1_y = y1 / CHUNK_SIZE, chunk1_x = x1 / CHUNK_SIZE;
	const int chunk2_y = y2 / CHUNK_SIZE, chunk2_x = x2 / CHUNK_SIZE;

	const int inside_chunk1_y = y1 % CHUNK_SIZE, inside_chunk1_x = x1 % CHUNK_SIZE;
	const int inside_chunk2_y = y2 % CHUNK_SIZE, inside_chunk2_x = x2 % CHUNK_SIZE;

	Chunk& chunk1 = chunks[chunk1_y][chunk1_x];
	Chunk& chunk2 = chunks[chunk2_y][chunk2_x];

	const Element temp = chunk1.elements[inside_chunk1_y][inside_chunk1_x];
	chunk1.elements[inside_chunk1_y][inside_chunk1_x] = chunk2.elements[inside_chunk2_y][inside_chunk2_x];
	chunk2.elements[inside_chunk2_y][inside_chunk2_x] = temp;

	chunk1.is_active |= activate_chunk;
	chunk2.is_active |= activate_chunk;
}

const Element& FallingSandEngine::get_element_at(const int y, const int x) {
	if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT) {
		return Element(Substance::NOTHING);
	}

	const int chunk_y = y / CHUNK_SIZE, chunk_x = x / CHUNK_SIZE;
	Chunk& chunk = chunks[chunk_y][chunk_x];
	
	const int inside_chunk_y = y % CHUNK_SIZE, inside_chunk_x = x % CHUNK_SIZE;

	return chunk.elements[inside_chunk_y][inside_chunk_x];
}

void FallingSandEngine::set_element_at(const int y, const int x, const Element &element, const bool activate_chunk) {
	if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT) {
		return;
	}

	const int chunk_y = y / CHUNK_SIZE, chunk_x = x / CHUNK_SIZE;
	Chunk& chunk = chunks[chunk_y][chunk_x];

	const int inside_chunk_y = y % CHUNK_SIZE, inside_chunk_x = x % CHUNK_SIZE;

	chunk.elements[inside_chunk_y][inside_chunk_x] = element;

	chunk.is_active |= activate_chunk;
}

FallingSandEngine::~FallingSandEngine() {
	for (int i = 0; i < CHUNK_NUM_WIDTH; i++) {
		delete[] chunks[i];
	}
	delete[] chunks;
}

void FallingSandEngine::draw_world_on_texture(sf::Uint8* screen_pixels) {
	sf::Color* pixel_ptr = (sf::Color *) screen_pixels;

	for (int y = 0; y < CHUNK_NUM_HEIGHT; y++) {
		for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
			for (int i = 0; i < CHUNK_SIZE; i++) {
				for (int j = 0; j < CHUNK_SIZE; j++) {
					const Element& element = chunks[y][x].get_element_at(i, j);
					
					pixel_ptr[(y * CHUNK_SIZE + i) * WORLD_WIDTH + x * CHUNK_SIZE + j] = color_of_substance[(int) element.substance];
				}
			}
		}
	}
}
