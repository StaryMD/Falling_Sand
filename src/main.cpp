
#include "gameEngine.cpp"

int main() {
	GameEngine gameEngine(WORLD_HEIGHT, WORLD_WIDTH);

	gameEngine.setup();

	while (gameEngine.window.isOpen()) {
		gameEngine.update();
	}

	return 0;
}
