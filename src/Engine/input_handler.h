#pragma once

#include <functional>
#include <vector>
#include "window.h"

class InputHandler {

public:
	InputHandler(Window* window);
	void attach(Window* window);
	void connect(int key, std::function<void ()> func);
	void connect(std::vector<int> keys, std::function<void ()> func);
	void disconnect(int key);
	void disconnect(std::vector<int> keys);
	void listen();

private:
	Window* attached_window;
	std::vector<std::pair<int, std::function<void()>>> binds;
	std::vector<std::pair<std::vector<int>, std::function<void()>>> binds_m;
};