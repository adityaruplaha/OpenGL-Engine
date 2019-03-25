#pragma once

#include <fstream>
#include <map>
#include <string>
#include "base/base.h"

struct UniformMetadata
{
	std::string model_mat_name;
	std::string view_mat_name;
	std::string proj_mat_name;
};

class Shader
{
public:
	static Shader* create(std::ifstream stream, int type);	
	~Shader();

	void compile();
	GLuint get();

	bool is_compiled = false;

	UniformMetadata metadata;

private:
	Shader(std::string& src, int type);

	std::string src;
	GLuint shader;
	const int type;

	static std::string getShaderName(int type);

	static std::map<std::string, Shader*> shader_cache;
};

