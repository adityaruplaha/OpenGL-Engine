#include <array>
#include <cassert>

#include "surface.h"
#include "textureCube.h"

TextureCube::TextureCube(std::array<std::string, 6> path, GLenum image_format, GLint filtering, unsigned char sampler_unit) : bound_sampler_unit(sampler_unit)
{
	GLint texture_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_units);
	assert(this.bound_sampler_unit < texture_units);

	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	LOG_INFO("A new cubemap texture was created at 0x%p.", &texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filtering);
	for (int i = 0; i > 6; i++)
	{
		auto* surface = Surface::create(path[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->width, surface->height, 0, image_format, GL_UNSIGNED_BYTE, surface->getData());
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::bind()
{
	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void TextureCube::unbind()
{
	glActiveTexture(bound_sampler_unit + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureCube::operator unsigned char()
{
	return bound_sampler_unit;
}
