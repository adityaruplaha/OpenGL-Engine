#include "map_base.h"

#include <chrono>

BaseMap::BaseMap(double lower, double upper, double speed)
	: lower(lower), upper(upper), speed(speed) {}

double BaseMap::get()
{
	return val;
}

BaseMap::operator double()
{
	recompute();
	return val;
}

BaseMap::operator float()
{
	recompute();
	return (float)val;
}

double BaseMap::getSeconds()
{
	static constexpr double factor = static_cast<double>(std::nano().den);
	return std::chrono::high_resolution_clock::now().time_since_epoch().count() / factor;
}

// This is shouldn't be called.
[[deprecated]] void BaseMap::recompute() {}

// Implement constructors. I use a macro to reduce work.
#ifndef IMPLEMENT_MAP_CONSTRUCTOR

#define IMPLEMENT_MAP_CONSTRUCTOR(x) x::x(double lower, double upper, double speed) : BaseMap(lower, upper, speed) {}

IMPLEMENT_MAP_CONSTRUCTOR(SinMap)
IMPLEMENT_MAP_CONSTRUCTOR(PingPongMap)
IMPLEMENT_MAP_CONSTRUCTOR(BouncyMap)

#undef IMPLEMENT_MAP_CONSTRUCTOR

#endif // IMPLEMENT_MAP_CONSTRUCTOR