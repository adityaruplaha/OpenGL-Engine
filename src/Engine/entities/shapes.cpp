#include <Common/log.h>
#include "shapes.h"

constexpr auto PI = 3.141592653589793238462643383279502884L;
constexpr auto conversionFactor = PI / 180;

// Whether to check for a reasonable value of n.
// STRONGLY RECOMMENDED to keep at the default value = 'true'
constexpr bool N_CHECK = true;

// Where to block vertex compuation completely with an assertion.
// Defaults to 1000.
constexpr int ASSERT = 1000;

// Warn at n greater than...
// Defaults to 500.
constexpr int WARN = 500;

// >> 2D STUFF

Vertices computeVertices2D(int n, float r, double offset)
{
	if (N_CHECK)
	{
		assert(n < ASSERT && "Too many sides supplied.");
		if (n > WARN)
		{
			LOG_WARNING("Computing vertices for n = %i is not recommended", n);
		}
	}
	Vertices v;
	const double increment = 360.0 / n;
	for (double a = 90 + offset; a < 450 + offset; a += increment)
	{
		auto a_ = a * conversionFactor;
		v.push_back({ {r*cosf(a_), r*sinf(a_), 0.0f} });
	}
	return std::move(v);
}

RPolygon::RPolygon(int sides, float r, double offset = 0) : Object(computeVertices2D(sides, r, offset), GL_TRIANGLE_FAN) {};