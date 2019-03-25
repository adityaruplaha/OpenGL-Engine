#pragma once

#include <vector>
#include "base/base.h"

class VBO
{
public:
	VBO(std::vector<float> buffer_data, std::vector<int> slice_attribs, unsigned int stride = 0);
	void bind();
	GLuint* get();

	static void unbind();

private:
	void slice_using(std::vector<int> attribs, unsigned int stride);
	void copyFrom(std::vector<float> data);
	float* buffer_data_internal;

	GLuint vbo;
};