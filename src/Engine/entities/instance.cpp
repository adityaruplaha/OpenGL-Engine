#include <GLCore/program.h>
#include "instance.h"

Instance::Instance(Object* obj, Orientaion o) : obj(obj), orientation(o) {
    obj->instance_count++;
}

// Controls whether to uninitialize transformation matrix after every rendering instance.
// Usually unnecessary. You can try to
constexpr bool POST_RENDER_UNINIT_MODEL_MAT = false;

void Instance::render() {
    Program::current_program->setMat4(Program::current_program->metadata.model_mat_name,
                                      calculateModelMat());
    obj->render();
    if (POST_RENDER_UNINIT_MODEL_MAT) {
        Program::current_program->setMat4(Program::current_program->metadata.model_mat_name,
                                          std::move(glm::mat4(1.0f)));
    }
}

glm::mat4 Instance::calculateModelMat() {
    auto& [position, rotation, scale] = orientation;
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::rotate(mat, glm::radians(rotation.x), {1, 0, 0});
    mat = glm::rotate(mat, glm::radians(rotation.y), {0, 1, 0});
    mat = glm::rotate(mat, glm::radians(rotation.z), {0, 0, 1});
    mat = glm::scale(mat, scale);
    mat = glm::translate(mat, position);
    return std::move(mat);
}
