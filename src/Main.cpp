#include <filesystem>
#include <string_view>

#include "FallingSandEngine.hpp"

int32_t main(const int32_t /*argc*/, const char** argv) {
  const std::string_view executable_path(argv[0]);
  std::filesystem::current_path(std::filesystem::path(executable_path).parent_path());

  FallingSandEngine falling_engine("Falling Sand Simulator", {constants::kWorldWidth, constants::kWorldHeight});

  falling_engine.Run();

  return 0;
}
