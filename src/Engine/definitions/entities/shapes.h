#include <cmath>

#include "object.h"

class RPolygon : public Object
{
public:
	explicit RPolygon(int sides, float r, double offset);
};