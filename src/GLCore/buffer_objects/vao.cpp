#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &vao);
    LOG_SUCCESS("New VAO created at %p.", &vao);
}

void VAO::bind() {
    glBindVertexArray(vao);
}

void VAO::unbind() {
    glBindVertexArray(0);
}
