#include <Common\log.h>

#include "object.h"

// 0xF = 15 is just a shit value that will trigger an error in render() anyways

Object::Object(Vertices v, Indices i, std::vector<int> slices) : v(std::move(v)), indexSize(i.size()), renderMode(0xF)
{
	LOG_INFO("New object created at memory address 0x%p.", this);
	vao = VAO();
	vao.bind();
	if (slices.empty())
	{
		LOG_WARNING("0x%p: Slicing attributes not set! Determining them programmatically...", this);
		for (Attribute attrib : (this->v)[0])
		{
			slices.push_back(attrib.size());
		}
	}
	VBO vbo = VBO(std::move(flatten()), slices);
	EBO ebo = EBO(i);
}

Object::Object(Vertices v, unsigned char renderMode, std::vector<int> slices) : v(std::move(v)), indexSize(0), renderMode(renderMode)
{
	LOG_INFO("New object created at memory address 0x%p.", this);
	vao = VAO();
	vao.bind();
	if (slices.empty())
	{
		LOG_WARNING("0x%p: Slicing attributes not set! Determining them programmatically...", this);
		for (Attribute attrib : (this->v)[0])
		{
			slices.push_back(attrib.size());
		}
	}
	VBO vbo = VBO(std::move(flatten()), slices);
	VAO::unbind();
}

void Object::render()
{
	vao.bind();
	if (indexSize)
	{
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	}
	else if (renderMode > 0x6)
	{
		LOG_CRITICAL("0x%p: Invalid rendering mode! Aborting draw...", this);
	}
	else
	{
		glDrawArrays(renderMode, 0, v.size());
	}
	VAO::unbind();
}

std::vector<float> Object::flatten()
{
	std::vector<float> t;
	for (auto& vertex : this->v)
	{
		for (auto& attrib : vertex)
		{
			for (auto& val : attrib)
			{
				t.push_back(val);
			}
		}
	}
	return t;
}