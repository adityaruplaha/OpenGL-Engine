#include <iostream>
#include <chrono>

#include <Common/map_funcs/map_base.h>
#include <Common/log.h>

#include <GLCore/program.h>
#include <GLCore/textures/texture2D.h>

#include "definitions/window.h"
#include "definitions/input_handler.h"
#include "definitions/entities/instance.h"
#include "definitions/entities/shapes.h"
#include "definitions/entities/camera.h"

int main()
{
	Initializer::InitGL();
	REGISTER_LOG_FILE(new std::ofstream("../../logs/log.log"));
	auto* window = new Window(600, 600, "OpenGL Engine 0.0.1", {45.0f, 0.1f, 100.0f});
	Initializer::GLAD_Init();
	
	auto* window_ptr = window->getWindowPointer();
	auto* handler = new InputHandler(window_ptr);

	handler->connect(GLFW_KEY_ESCAPE,
		[&window_ptr]() {glfwSetWindowShouldClose(window_ptr, true); }
	);

	Vertices vertices = {
	{ { -0.5f,  0.5f, 0.0f } , { 0.0f, 0.0f, 1.0f } , {0.0f, 1.0f} },  // top left
	{ {  0.5f,  0.5f, 0.0f } , { 1.0f, 0.0f, 0.0f } , {1.0f, 1.0f} },  // top right
	{ {  0.5f, -0.5f, 0.0f } , { 1.0f, 1.0f, 0.0f } , {1.0f, 0.0f} },  // bottom right
	{ { -0.5f, -0.5f, 0.0f } , { 0.0f, 1.0f, 0.0f } , {0.0f, 0.0f} },  // bottom left
	};
	
	auto* rect = new Object(vertices, GL_TRIANGLE_FAN);

	auto* instance = new Instance(rect);
	
	auto* vs = Shader::create(std::ifstream("../../shaders/tex_mat.vert"), GL_VERTEX_SHADER);
	auto* fs = Shader::create(std::ifstream("../../shaders/tex2.frag"), GL_FRAGMENT_SHADER);

	auto* prog = new Program(2, vs, fs);
	prog->link();
	prog->start();

	window->setup();

	auto* container = new Texture2D("../../res/container.jpg", GL_RGB, GL_LINEAR, 0);
	container->bind();
	auto* smile = new Texture2D("../../res/awesomeface.png", GL_RGBA, GL_LINEAR, 1);
	smile->bind();
	prog->setInt("texture0", *container);
	prog->setInt("texture1", *smile);

	CameraOrientation o{ glm::vec3(0, 0, 3), Directions::INTO_SCREEN, 0.0f, 1.0f };
	auto* cam = new Camera(o);
	cam->start();

	auto mix_ratio = PingPongMap(0, 1, 0.5);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (!glfwWindowShouldClose(window_ptr))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		handler->listen();

		prog->setFloat("ratio", mix_ratio);
		instance->orientation.position.z = mix_ratio;
		instance->orientation.rotation.x -= 0.5f;
		instance->orientation.rotation.y += 0.37f;

		instance->render();

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}

	cam->stop();
	LOG_INFO("Terminating...");
	glfwTerminate();
	return 0;
}