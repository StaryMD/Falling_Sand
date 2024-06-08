#include "RandomNumberGenerators.hpp"

#include <cstdint>
#include <cstdlib>
#include <ctime>

FastRng::FastRng(const int32_t seed) {
  seed_ = seed;
}

FastRng::FastRng() {
  std::srand(std::time(nullptr));
  seed_ = std::rand();
}

int32_t FastRng::NextValue() {
  seed_ = 75 * seed_ + 74;
  seed_ %= (1 << 16) + 1;
  return seed_;
}
