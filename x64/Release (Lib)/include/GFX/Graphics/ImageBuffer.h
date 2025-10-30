#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include <string>
#include "../Utils.h"

enum class ImageBufferFormat
{
	RGBA,
	RGB,
	GRAY
};

class ImageBuffer : public Buffer
{
private:
	void calcImageSize(int width, int height, ImageBufferFormat format);
	void createImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);
	VkFormat getVkFormat(ImageBufferFormat format);

public:
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkDeviceSize imageSize;
	int descriptorIndex = -1;

	ImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);
	ImageBuffer(std::vector<unsigned char> imageData, uint32_t width, uint32_t height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, ImageBufferFormat format);
	void dispose(VkDevice device);
};