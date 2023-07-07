#include "RandomNumberGenerators.hpp"

#include <cstdlib>
#include <ctime>

FastRng::FastRng() {
  std::srand(std::time(nullptr));
  seed_ = std::rand();
}

int FastRng::NextRandValue() {
  seed_ = 214013 * seed_ + 2531011;  //NOLINT
  seed_ = (seed_ >> 16) & 0x7FFF;    //NOLINT
  return seed_;
}
