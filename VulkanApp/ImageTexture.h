#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "Utils.h"

class ImageTexture : public Buffer
{
private:
	void loadImageData(std::string filename);

public:
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkDeviceSize imageSize;
	int descriptorIndex = -1;
	int width;
	int height;
	int channels;
	stbi_uc* imageData;

	ImageTexture(std::string filename);
	void init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);
	void dispose(VkDevice device);
};

