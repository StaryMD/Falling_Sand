#pragma once

#include "World.hpp"

#define DEFINE_NEW_LAW(law_name) void law_name(World& world, const int y, const int x)

DEFINE_NEW_LAW(law_for_NOTHING);
DEFINE_NEW_LAW(law_for_AIR);
DEFINE_NEW_LAW(law_for_SAND);
DEFINE_NEW_LAW(law_for_STONE);

#pragma GCC diagnostic ignored "-Wunused-variable"
static void (*laws[SUBSTANCE_COUNT])(World& world, const int y, const int x) = {
	law_for_NOTHING,
	law_for_AIR,
	law_for_SAND,
	law_for_STONE,
};
