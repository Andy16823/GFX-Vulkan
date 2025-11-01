#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include "Scene.h"
#include "Sprite.h"
#include "Model.h"

class GFX
{
public:
	/// <summary>
	/// Factory method to create a window
	/// </summary>
	/// <param name="wName"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	static GLFWwindow* createWindow(std::string wName = "GFX C++", const int width = 800, const int height = 600);

	/// <summary>
	/// Factory method to create a sprite
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static std::unique_ptr<Sprite> createSprite(const std::string& texturePath, const std::string& name = "Sprite");

	/// <summary>
	/// Factory method to create a model
	/// </summary>
	/// <param name="modelPath"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static std::unique_ptr<Model> createModel(const std::string& modelPath, const std::string& name = "Model");
};

