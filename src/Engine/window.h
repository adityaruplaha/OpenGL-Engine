#pragma once

#include <map>
#include <string>
#include <GLCore/base/base.h>

struct PerscectiveData
{
	float FOV;
	float NEAR_PLANE;
	float FAR_PLANE;
};

class Window {

public:

	Window(int width, int height, std::string name, PerscectiveData p = {});
	void setup();

	bool should_close();
	void close();

	void buffer_swap();

	GLFWwindow* getWindowPointer();

	int width;
	int height;

	static void clear();
	
private:
	PerscectiveData p;
	void perspective();

	void onResize(int w, int h);
	GLFWwindow* window;

	static std::map<GLFWwindow*, Window*> window_registry;
};