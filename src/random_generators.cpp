#include "random_generators.hpp"

#include <cstdlib>
#include <ctime>

FastRng::FastRng() {
	std::srand(std::time(NULL));
	seed = std::rand();
}

int FastRng::rand() {
	seed = 214013 * seed + 2531011;
	return (seed >> 16) & 0x7FFF;
}