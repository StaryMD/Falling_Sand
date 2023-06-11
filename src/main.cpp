#include "GameEngine.hpp"
#include "common_constants.hpp"

int main() {
  GameEngine game_engine("Falling Sand Simulator", kWorldWidth, kWorldHeight);

  game_engine.Run();

  return 0;
}
