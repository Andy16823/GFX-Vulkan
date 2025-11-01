#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Utils.h"

/// <summary>
/// ImageTexture class
/// The ImageTexture class is an container for image data loaded from a file
/// </summary>
class ImageTexture
{
public:
	int width;
	int height;
	stbi_uc* imageData;
	int bufferIndex = -1;

	ImageTexture(std::string file);
	ImageTexture(int width, int height, const std::vector<uint8_t>& pixelData);
	~ImageTexture();
	void freeImageData();
};

