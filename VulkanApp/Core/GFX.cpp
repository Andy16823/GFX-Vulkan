#include "GFX.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

GLFWwindow* GFX::createWindow(std::string wName, const int width, const int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	return window;
}