#include "RandomNumberGenerators.hpp"

#include <cstdlib>
#include <ctime>

FastRng::FastRng(const int seed) {
  seed_ = seed;
}

FastRng::FastRng() {
  std::srand(std::time(nullptr));
  seed_ = std::rand();
}

int FastRng::NextValue() {
  seed_ = 214013 * seed_ + 2531011;
  seed_ = (seed_ >> 16) & 0x7FFF;
  return seed_;
}
