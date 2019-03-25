#pragma once

#include <map>
#include <string>
#include <GLCore\base\base.h>

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
	GLFWwindow* getWindowPointer();

	int width;
	int height;
	
private:
	PerscectiveData p;
	void perspective();

	void onResize(int w, int h);
	GLFWwindow* window;

	static std::map<GLFWwindow*, Window*> window_registry;
};