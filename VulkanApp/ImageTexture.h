#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Utils.h"

class ImageTexture
{
public:
	int width;
	int height;
	stbi_uc* imageData;
	int bufferIndex = -1;

	ImageTexture(std::string file);
	~ImageTexture();
	void freeImageData();
};

