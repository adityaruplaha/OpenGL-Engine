#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

inline glm::vec3 operator*(glm::mat4 m, glm::vec3 v) {
    return std::move(glm::vec3(m * glm::vec4(v, 1.0f)));
}
