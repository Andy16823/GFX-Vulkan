#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "../Utils.h"

class ImageBuffer : public Buffer
{
private:
	void createImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);

public:
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkDeviceSize imageSize;
	int descriptorIndex = -1;

	ImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);
	void dispose(VkDevice device);
};