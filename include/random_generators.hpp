#pragma once

#include <cstdlib>
#include <ctime>

class FastRng {
	int seed;

public:
	FastRng();

	int rand();
	
};