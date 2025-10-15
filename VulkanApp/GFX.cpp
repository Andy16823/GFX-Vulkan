#include "GFX.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow* GFX::createWindow(std::string wName, const int width, const int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
	return window;
}

std::unique_ptr<Scene> GFX::createScene()
{
	return std::make_unique<Scene>();
}

std::unique_ptr<Sprite> GFX::createSprite(const std::string& texturePath, const std::string& name)
{
	return std::make_unique<Sprite>(name, texturePath);
}

std::unique_ptr<Model> GFX::createModel(const std::string& modelPath, const std::string& name)
{
	return std::make_unique<Model>(name, modelPath);
}