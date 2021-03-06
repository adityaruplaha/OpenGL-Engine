#include <chrono>
#include <iostream>
#include <Common/cmake_vars.h>
#include <Common/log.h>
#include <Common/map_funcs/map_base.h>
#include <GLCore/program.h>
#include <GLCore/textures/texture2D.h>
#include "entities/camera.h"
#include "entities/instance.h"
#include "entities/shapes.h"
#include "input_handler.h"
#include "window.h"

int main() {
    Initializer::InitGL();
    REGISTER_LOG_FILE(new std::ofstream(PROJECT_ROOT_DIR + "logs/log.log"));

    LOG_INFO("Initialized program.");
    auto version = GIT_BRANCH + "@" + GIT_COMMIT;
    LOG_INFO("Version: %s.", version.c_str());

    auto* window = new Window(600, 600, "OpenGL Engine " + version, {45.0f, 0.1f, 100.0f});
    Initializer::GLAD_Init();
    auto* handler = new KeyboardInputHandler(window);

    handler->connect(GLFW_KEY_ESCAPE, [&window]() { window->close(); });

    Vertices vertices = {
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // top left
        {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},   // top right
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // bottom right
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // bottom left
    };

    auto* rect = new Object(vertices, GL_TRIANGLE_FAN);
    auto* instance = new Instance(rect);

    auto* vs =
        Shader::create(std::ifstream(PROJECT_ROOT_DIR + "shaders/tex_mat.vert"), GL_VERTEX_SHADER);
    auto* fs =
        Shader::create(std::ifstream(PROJECT_ROOT_DIR + "shaders/tex2.frag"), GL_FRAGMENT_SHADER);
    auto* prog = new Program(2, vs, fs);

    prog->link();
    prog->start();

    window->setup();

    auto* container = new Texture2D(PROJECT_ROOT_DIR + "res/container.jpg", GL_RGB, GL_LINEAR, 0);
    container->bind();

    auto* smile = new Texture2D(PROJECT_ROOT_DIR + "res/awesomeface.png", GL_RGBA, GL_LINEAR, 1);
    smile->bind();

    prog->setInt("texture0", *container);
    prog->setInt("texture1", *smile);

    CameraOrientation o{glm::vec3(0, 0, 2), {Directions::UP, Directions::RIGHT}, 1.0f};

    auto* cam = new Camera(o);
    cam->start();

    handler->connect(GLFW_KEY_W, [&cam]() { cam->rmoveFD(); });
    handler->connect(GLFW_KEY_S, [&cam]() { cam->rmoveBK(); });
    handler->connect(GLFW_KEY_Q, [&cam]() { cam->rmoveLT(); });
    handler->connect(GLFW_KEY_E, [&cam]() { cam->rmoveRT(); });

    handler->connect(GLFW_KEY_A, [&cam]() { cam->rturnLT(); });
    handler->connect(GLFW_KEY_D, [&cam]() { cam->rturnRT(); });

    auto camera_y = SinMap(0.3, 8, 0.4);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    prog->setFloat("ratio", 0.7f);

    while (!window->should_close()) {
        Window::clear();
        handler->listen();

        instance->render();

        Camera::flush();
        window->buffer_swap();
    }

    cam->stop();

    LOG_INFO("Terminating...");
    glfwTerminate();
    return 0;
}
