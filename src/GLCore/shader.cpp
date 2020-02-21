#include <sstream>
#include <iostream>

#include "shader.h"

std::map<std::string, Shader*> Shader::shader_cache{};

Shader* Shader::create(std::ifstream stream, int type)
{
	std::ostringstream ss;
	ss << stream.rdbuf();
	std::string src = ss.str();
	auto iterator = shader_cache.find(src);
	if (iterator != shader_cache.end())
	{ 
		return iterator->second;
	}
	return new Shader(src, type);
}

Shader::Shader(std::string& src, int type) : src(src), type(type)
{
	shader = glCreateShader(this->type);
	LOG_INFO("A new %s was created at %p.", getShaderName(this->type).c_str(), &shader);
	if (this->src.empty())
	{
		LOG_ERROR("%p: Shader is blank!", &shader);
	}

	shader_cache[src] = this;
}

Shader::~Shader()
{
	shader = glCreateShader(type);
	LOG_INFO("%p: Shader deleted.", getShaderName(type), &shader);

	glDeleteShader(shader);
}

void Shader::compile()
{
	auto start_iter = src.find("#version");
	
	auto meta_pos = src.find("#meta");
	if (meta_pos < start_iter)
	{
		auto metadata_string = src.substr(meta_pos, start_iter - meta_pos - 1);
		auto model_iter = metadata_string.find("model: ") + 7; // size: 7
		auto view_iter = metadata_string.find("view: ") + 6;   // size: 6
		auto proj_iter = metadata_string.find("proj: ") + 6;   // size: 6

		try
		{
			metadata.model_mat_name =
				metadata_string.substr(model_iter, metadata_string.substr(model_iter).find(";"));
		}
		catch (std::out_of_range)
		{
			metadata.model_mat_name = "";
		}

		try
		{
			metadata.view_mat_name =
				metadata_string.substr(view_iter, metadata_string.substr(view_iter).find(";"));
		}
		catch (std::out_of_range)
		{
			metadata.view_mat_name = "";
		}

		try
		{
			metadata.proj_mat_name =
				metadata_string.substr(proj_iter, metadata_string.substr(proj_iter).find(";"));
		}
		catch (std::out_of_range)
		{
			metadata.proj_mat_name = "";
		}		
	}

	auto real_src = src.substr(start_iter);
	auto src_char_pointer = real_src.c_str();
	glShaderSource(shader, 1, &src_char_pointer, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		LOG_ERROR("Shader compilation for %s at %p failed.\n%s", getShaderName(type).c_str(), &shader, infoLog);
		return;
	}
	LOG_SUCCESS("%s at %p was compiled successfully.", getShaderName(type).c_str(), &shader);
	is_compiled = true;
}
GLuint Shader::get()
{
	return shader;
}

std::string Shader::getShaderName(int type)
{
	switch (type)
	{
	case(GL_VERTEX_SHADER):
		return "VERTEX_SHADER";
	case(GL_FRAGMENT_SHADER):
		return "FRAGMENT_SHADER";
	case(GL_GEOMETRY_SHADER):
		return "GEOMETRY_SHADER";
	case(GL_TESS_CONTROL_SHADER):
		return "TESSELATION_CONTROL_SHADER";
	case(GL_TESS_EVALUATION_SHADER):
		return "TESSELATION_EVALUATION_SHADER";
	case(GL_COMPUTE_SHADER):
		return "COMPUTE_SHADER";
	default:
		return "UNKNOWN_SHADER";
	}
}
