#include "FallingSandEngine.hpp"

FallingSandEngine::FallingSandEngine() {
}

void FallingSandEngine::setup() {

}

void FallingSandEngine::update() {
	world.update();
}

void FallingSandEngine::draw_world_on_texture(sf::Uint8* screen_pixels) {
	sf::Color* pixel_ptr = (sf::Color *) screen_pixels;

	for (int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH; i++) {
		*pixel_ptr = color_of_substance[(int) world.grid[i]];
		pixel_ptr++;
	}
}

FallingSandEngine::~FallingSandEngine() {
}
