#pragma once

#include <functional>
#include <vector>
#include "window.h"

class InputHandler {

public:
    void attach(Window* wndow);
    virtual void listen() = 0;

protected:
    Window* attached_window;
};

class KeyboardInputHandler : InputHandler {

public:
    KeyboardInputHandler(Window* window);
    void connect(int key, std::function<void()> func);
    void connect(std::vector<int> keys, std::function<void()> func);
    void disconnect(int key);
    void disconnect(std::vector<int> keys);
    void listen() override;

private:
    std::vector<std::pair<int, std::function<void()>>> binds;
    std::vector<std::pair<std::vector<int>, std::function<void()>>> binds_m;
};
