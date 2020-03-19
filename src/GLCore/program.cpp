#include <cstdarg>
#include "program.h"

Program* Program::current_program{};

Program::Program() : metadata{"", "", ""} {
    program = glCreateProgram();
    LOG_INFO("A new shader program was created at %p.", &program);
}

Program::Program(int shader_count, ...) : metadata{"", "", ""} {
    program = glCreateProgram();
    LOG_INFO("A new shader program was created at %p.", &program);

    va_list args;
    va_start(args, shader_count);
    for (int i = 0; i < shader_count; i++) {
        auto shader = va_arg(args, Shader*);
        attach(shader);
    }
    va_end(args);
}

Program::~Program() {
    glDeleteProgram(program);
}

void Program::attach(Shader* shader) {
    shaders.push_back(shader);
}

void Program::link() {
    for (auto* shader : shaders) {
        if (!shader->is_compiled) {
            LOG_WARNING("%p: A shader wasn't pre-compiled. Compiling it now...", &program);
            shader->compile();
            if (!shader->is_compiled) {
                LOG_CRITICAL("%p: Shader couldn't be compiled!", &program);
            }
        }
        glAttachShader(program, shader->get());

        // Now go for uniform metadata parsing
        parseMetadata(shader);
    }

    glLinkProgram(program);

    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        LOG_ERROR("Program at %p failed to be linked.\n%s", &program, infoLog);
        return;
    }
    LOG_SUCCESS("Program at %p was linked successfully.", &program);
}

void Program::start() {
    current_program = this;
    glUseProgram(program);
}

void Program::stop() {
    current_program = nullptr;
    glUseProgram(0);
}

void Program::setBool(std::string name, bool b) {
    glUniform1i(getUniformLocation(name), (int)b);
}

void Program::setFloat(std::string name, float f) {
    glUniform1f(getUniformLocation(name), f);
}

void Program::setInt(std::string name, int i) {
    glUniform1i(getUniformLocation(name), i);
}

void Program::setVec2(std::string name, glm::vec2 v) {
    glUniform2f(getUniformLocation(name), v.x, v.y);
}

void Program::setVec3(std::string name, glm::vec3 v) {
    glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
}

void Program::setVec4(std::string name, glm::vec4 v) {
    glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Program::setMat4(std::string name, glm::mat4 m) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}

int Program::getUniformLocation(std::string name) {
    if (name == "") {
        return -1;
    }
    auto result = glGetUniformLocation(program, name.c_str());
    if (result == -1) {
        LOG_WARNING("%p: Invalid uniform name: '%s'", &program, name.c_str());
    }
    return result;
}

void Program::parseMetadata(Shader*& shader) {
    if (metadata.model_mat_name.empty() ||
        metadata.model_mat_name == shader->metadata.model_mat_name) {
        metadata.model_mat_name = shader->metadata.model_mat_name;
    } else {
        if (!shader->metadata.model_mat_name.empty())
            LOG_ERROR("%p: Model matrix uniform name mismatch!", &program);
    }

    if (metadata.view_mat_name.empty() ||
        metadata.view_mat_name == shader->metadata.view_mat_name) {
        metadata.view_mat_name = shader->metadata.view_mat_name;
    } else {
        if (!shader->metadata.view_mat_name.empty())
            LOG_ERROR("%p: View matrix uniform name mismatch!", &program);
    }

    if (metadata.proj_mat_name.empty() ||
        metadata.proj_mat_name == shader->metadata.proj_mat_name) {
        metadata.proj_mat_name = shader->metadata.proj_mat_name;
    } else {
        if (!shader->metadata.proj_mat_name.empty())
            LOG_ERROR("%p: Projection matrix uniform name mismatch!", &program);
    }
}
