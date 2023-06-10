#include "GameEngine.hpp"
#include "common_constants.hpp"

int main() {
  GameEngine gameEngine("Falling Sand Simulator", WORLD_WIDTH, WORLD_HEIGHT);

  gameEngine.run();

  return 0;
}
