#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include "FontAtlas.h"

/// <summary>
/// Texture buffer for 1 channel data (e.g., grayscale).
/// </summary>
class FontTextureBuffer : public Buffer
{
private:
	
	void createImageView(VkDevice device);

public:
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkDeviceSize imageSize;

	int descriptorIndex = -1;
	FontTextureBuffer(const FontAtlas& fontAtlas, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool);
	~FontTextureBuffer() = default;
	void dispose(VkDevice device);
	
};

