#include <iostream>
#include "Renderer.h"
#include <string>

GLFWwindow* window;
Renderer renderer;

GLFWwindow& CreateWindow(std::string wName = "GFX C++", const int width = 800, const int height = 600) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window =  glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	return *window;
}

int main() {
	window = &CreateWindow();
	
	// Init the vulkan renderer
	if (renderer.init(window) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	renderer.dispose();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}