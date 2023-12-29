#include <filesystem>
#include <string_view>

#include "GameEngine.hpp"

int32_t main(const int32_t /*argc*/, const char** argv) {
  const std::string_view executable_path(argv[0]);
  std::filesystem::current_path(std::filesystem::path(executable_path).parent_path());

  GameEngine game_engine("Falling Sand Simulator");

  game_engine.Run();

  return 0;
}
