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

void FallingSandEngine::apply_law(const Substance substance, const int y, const int x) {
	switch (substance) {
		case Substance::NOTHING: {
			law_for_NOTHING(y, x);
			break;
		}
		case Substance::AIR: {
			law_for_AIR(y, x);
			break;
		}
		case Substance::SAND: {
			law_for_SAND(y, x);
			break;
		}
		case Substance::STONE: {
			law_for_STONE(y, x);
			break;
		}
	}
}
	
void FallingSandEngine::update() {
    for (int y = 0; y < CHUNK_NUM_HEIGHT; y++) {
        for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
			Chunk &chunk = chunks[y][x];

            if (chunk.is_active) {
				chunk.is_active = false;
				bool there_is_more = false;
				
				for (int i = 0; i < CHUNK_SIZE; i++) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						const Substance substance = chunk.get_element_at(i, j).substance;
						
						if (SUBS_IS_INVERSELY_UPDATED(substance) == false) {
							apply_law(substance, y * CHUNK_SIZE + i, x * CHUNK_SIZE + j);
						}
						else {
							there_is_more = true;
						}
					}
				}

				chunk.is_active = there_is_more;
			}
        }
    }

    for (int y = CHUNK_NUM_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < CHUNK_NUM_WIDTH; x++) {
			Chunk &chunk = chunks[y][x];

            if (chunk.is_active) {
				chunk.is_active = false;

				for (int i = CHUNK_SIZE - 1; i >= 0; i--) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						const Substance substance = chunk.get_element_at(i, j).substance;
						
						if (SUBS_IS_INVERSELY_UPDATED(substance) == true) {
							apply_law(substance, y * CHUNK_SIZE + i, x * CHUNK_SIZE + j);
						}
					}
				}
			}
        }
    }
}

void FallingSandEngine::set_cell(const int y, const int x, const Element &element, bool activate_chunk) {
	set_element_at(y, x, element, activate_chunk);
}

void FallingSandEngine::swap_elements(const int y1, const int x1, const int y2, const int x2, bool activate_chunk) {
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

Element FallingSandEngine::get_element_at(const int y, const int x) const {
	const int chunk_y = y / CHUNK_SIZE, chunk_x = x / CHUNK_SIZE;
	const Chunk& chunk = chunks[chunk_y][chunk_x];
	
	const int inside_chunk_y = y % CHUNK_SIZE, inside_chunk_x = x % CHUNK_SIZE;

	return chunk.elements[inside_chunk_y][inside_chunk_x];
}

void FallingSandEngine::set_element_at(const int y, const int x, const Element &element, bool activate_chunk) {
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
