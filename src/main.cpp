#include "CommonConstants.hpp"
#include "GameEngine.hpp"

int main() {
  GameEngine game_engine("Falling Sand Simulator", kWorldWidth, kWorldHeight);

  game_engine.Run();

  return 0;
}
