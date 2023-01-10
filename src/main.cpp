#include "FallingSandEngine.hpp"
#include "GameEngine.hpp"

int main() {
	GameEngine gameEngine(WORLD_WIDTH, WORLD_HEIGHT);
	
	gameEngine.run();

	return 0;
}
