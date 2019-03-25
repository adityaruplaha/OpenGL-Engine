#pragma once

#include <cassert>
#include <vector>

#include <GLCore/base/base.h>

using Attribute = std::vector<float>;
using Vertex = std::vector<Attribute>;
using Vertices = std::vector<Vertex>;

using Indices = std::vector<GLuint>;

Attribute operator +(Attribute& a, Attribute& b);
Attribute operator -(Attribute& a, Attribute& b);
