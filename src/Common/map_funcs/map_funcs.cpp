#include "map_base.h"

#include <cmath>

void SinMap::recompute()
{
	double seconds = getSeconds();

	val = sin(seconds * speed * 2);
	// val is between -1 & 1

	val += 1;
	// val is between 0 & 2

	val /= 2;
	// val is between 0 & 1

	val *= (upper - lower);
	// val is between 0 & (upper - lower)

	val += lower;
	// val is between lower & upper
}

void PingPongMap::recompute()
{
	double seconds = getSeconds();

	val = seconds * speed;
	while (val > 2)
	{
		val -= 2;
	}
	// val between 0 & 2

	// magic function: y = -|x-1|+1
	// forms nice triangle for values [0, 2]
	val = -abs(val - 1) + 1;
	// val between 0 & 1

	val *= (upper - lower);
	// val is between 0 & (upper - lower)

	val += lower;
	// val is between lower & upper
}

void BouncyMap::recompute()
{
	double seconds = getSeconds();

	val = seconds * speed * 2;
	while (val > 2)
	{
		val -= 2;
	}
	// val between 0 & 2

	// magic function: y = x(2-x)
	// forms bounce for values in [0, 2]
	val = val*(2-val);
	// val between 0 & 1

	val *= (upper - lower);
	// val is between 0 & (upper - lower)

	val += lower;
	// val is between lower & upper
}