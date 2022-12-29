#pragma once

#include <cstdlib>
#include <ctime>

class FastRng {
	int seed;

public:
	FastRng() {
		std::srand(std::time(NULL));
		seed = std::rand();
	}

	inline int rand() {
		seed = 214013 * seed + 2531011;
		return (seed >> 16) & 0x7FFF;
	}
	
};