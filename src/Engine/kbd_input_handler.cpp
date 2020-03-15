#include <algorithm>
#include "input_handler.h"

void InputHandler::attach(Window* window)
{
	attached_window = window;
}

/// Specific stuff

KeyboardInputHandler::KeyboardInputHandler(Window* window)
{
	this->attach(window);
}

void KeyboardInputHandler::connect(int key, std::function<void()> func)
{
	auto pair = std::make_pair(key, func);
	binds.push_back(pair);
}

void KeyboardInputHandler::connect(std::vector<int> keys, std::function<void()> func)
{
	auto pair = std::make_pair(keys, func);
	binds_m.push_back(pair);
}

void KeyboardInputHandler::disconnect(int key)
{
	for (int i = 0; i < binds.size(); i++)
	{
		if (binds[i].first == key)
			binds.erase(binds.begin() + i);

		break;
	}
}

void KeyboardInputHandler::disconnect(std::vector<int> keys)
{
	for (int i = 0; i < binds_m.size(); i++)
	{
		if (binds_m[i].first == keys)
			binds_m.erase(binds_m.begin() + i);

		break;
	}
}

void KeyboardInputHandler::listen()
{
	auto* ptr = attached_window->getWindowPointer();
	for (auto b : binds)
	{
		if (glfwGetKey(ptr, b.first) == GLFW_PRESS)
			(b.second)();
	}

	for (auto b : binds_m)
	{
		bool active = true;
		for (auto k : b.first)
		{
			active = active && (glfwGetKey(ptr, k) == GLFW_PRESS);
		}
		if (active)
		{
			(b.second)();
		}
	}
}