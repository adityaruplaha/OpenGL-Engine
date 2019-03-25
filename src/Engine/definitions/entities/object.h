#pragma once

#include <GLCore/buffer_objects.h>
#include "meta/avv_types.h"

class Object
{
public:
	Object(Vertices v, Indices i, std::vector<int> = {});
	Object(Vertices v, unsigned char renderMode = GL_TRIANGLE_FAN, std::vector<int> slices = {});
	void render();

	unsigned long instance_count;

private:
	VAO vao;
	Vertices v;
	const unsigned int indexSize;
	const unsigned char renderMode;
	std::vector<float> flatten();
};