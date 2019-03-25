#include <GLCore/base/gl_math_defs.h>
#include <GLCore/program.h>
#include <Common/log.h>

#include "window.h"

std::map<GLFWwindow*, Window*> Window::window_registry{};

Window::Window(int width, int height, std::string name, PerscectiveData p) : p(p), width(width), height(height)
{
	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		LOG_CRITICAL("Failed to create GLFW window\n");
	}
	glfwMakeContextCurrent(window);
	window_registry[window] = this;
}

void Window::setup()
{
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* win, int w, int h)
	{
		window_registry[win]->onResize(w, h);
	});
	perspective();
}

GLFWwindow* Window::getWindowPointer()
{
	return window;
}

void Window::perspective()
{
	auto& [fov, n, f] = p;
	glm::mat4 proj = glm::mat4(1.0f);
	if (fov && n && f)
	{
		proj = glm::perspective(glm::radians(fov), (float)width / (float)height, n, f);
	}
	if (Program::current_program)
	{
		Program::current_program->setMat4(Program::current_program->metadata.proj_mat_name, proj);
		return;
	}
	LOG_ERROR("WINDOW: No shader program is bound! Unable to set projection matrix.");
}

void Window::onResize(int w, int h)
{
	width = w;
	height = h;
	perspective();

	glViewport(0, 0, w, h);
}
