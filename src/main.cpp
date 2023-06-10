#include "GameEngine.hpp"
#include "common_constants.hpp"

int main() {
  GameEngine game_engine("Falling Sand Simulator", WORLD_WIDTH, WORLD_HEIGHT);

  game_engine.run();

  return 0;
}
