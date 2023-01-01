#pragma once

#include <SFML/Graphics.hpp>

#include "World.hpp"

class FallingSandEngine {
public:

	FallingSandEngine();

	void setup();

	void update();

	void draw_world_on_texture(sf::Uint8* screen_pixels);

	void set_cell(const int y, const int x, Element element) {
		world.set_element_at(y, x, element);
	}

	~FallingSandEngine();

private:

	World world;

};
