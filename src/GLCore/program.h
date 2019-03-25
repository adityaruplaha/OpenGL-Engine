#pragma once

#include <vector>
#include "base/gl_math_defs.h"
#include "shader.h"

class Program
{
public:
	Program();
	Program(int shader_count, ...);
	~Program();

	void attach(Shader* shader);
	void link();

	void start();
	void stop();

	void setBool(std::string name, bool b);
	void setFloat(std::string name, float f);
	void setInt(std::string name, int i);
	void setVec2(std::string name, glm::vec2 v);
	void setVec3(std::string name, glm::vec3 v);
	void setVec4(std::string name, glm::vec4 v);
	void setMat4(std::string name, glm::mat4 v);

	static Program* current_program;

	UniformMetadata metadata;

private:
	int getUniformLocation(std::string name);
	void parseMetadata(Shader*& shader);
	GLuint program;
	std::vector<Shader*> shaders;
};
