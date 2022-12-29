#pragma once

#include <SFML/Graphics.hpp>

#include "world.hpp"

class FallingSandEngine {
public:

	FallingSandEngine();

	void setup();

	void update();

	void draw_world_on_texture(sf::Uint8* screen_pixels);

	void set_cell(const int y, const int x, Substance subs) {
		world.set_subs_at(y, x, subs);
	}

	~FallingSandEngine();

private:

	World world;

	sf::Color color_of_substance[SUBSTANCE_COUNT] = {
		sf::Color::Red,
		sf::Color(101, 101, 101),
		sf::Color::Yellow
	};

};
