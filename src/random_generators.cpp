#include "random_generators.hpp"

#include <cstdlib>
#include <ctime>

FastRng::FastRng() {
  std::srand(std::time(NULL));
  seed_ = std::rand();
}

int FastRng::rand() {
  seed_ = 214013 * seed_ + 2531011;
  return (seed_ >> 16) & 0x7FFF;
}