#pragma once

#include <string>

#include <GLCore/base/base.h>

class Texture2D {
public:
    Texture2D(std::string path, GLenum image_format, GLint filtering = GL_LINEAR,
              unsigned char bound_sampler_unit = 0);
    void bind();
    void unbind();
    operator unsigned char();

protected:
    GLuint texture;
    const GLenum bound_sampler_unit;
};
