#include "GFX.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

GLFWwindow* GFX::createWindow(std::string wName, const int width, const int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	m_windows.push_back(window);
	return window;
}

GLFWwindow* GFX::getWindow(int index)
{
	if (index < 0 || index >= m_windows.size()) {
		return nullptr;
	}
	return m_windows[index];
}
