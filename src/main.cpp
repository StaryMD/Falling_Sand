#include "FallingSandEngine.hpp"
#include "GameEngine.hpp"

int main() {
	FallingSandEngine fallingSandEngine;
	GameEngine gameEngine(fallingSandEngine, WORLD_WIDTH, WORLD_HEIGHT);
	
	gameEngine.run();

	return 0;
}
