#pragma once

#include <string>

#include <GLCore/base/base.h>

class TextureCube {
public:
	// Array must be in {right, left, top, bottom, back, front} format.
	TextureCube(std::array<std::string, 6> path, GLenum image_format, GLint filtering = GL_LINEAR, unsigned char bound_sampler_unit = 0);
	void bind();
	void unbind();
	operator unsigned char();

protected:
	GLuint texture;
	const GLenum bound_sampler_unit;
};