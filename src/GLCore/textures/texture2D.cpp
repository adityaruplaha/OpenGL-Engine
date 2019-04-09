#include <cassert>

#include "surface.h"
#include "texture2D.h"

Texture2D::Texture2D(std::string path, GLenum image_format, GLint filtering, unsigned char sampler_unit) : bound_sampler_unit(sampler_unit)
{
	GLint texture_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_units);
	assert(this->bound_sampler_unit < texture_units);

	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	LOG_INFO("A new 2D texture was created at 0x%p.", &texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	auto* surface = Surface::create(path);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width, surface->height, 0, image_format, GL_UNSIGNED_BYTE, surface->getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind()
{
	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture2D::unbind()
{
	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::operator unsigned char()
{
	return bound_sampler_unit;
}
