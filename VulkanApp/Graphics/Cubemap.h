#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "Buffer.h"
#include "../Utils.h"

struct CubemapFaceData {
	std::vector<stbi_uc*> faceData; // Array of 6 pointers to image data for each face
	uint32_t width;
	uint32_t height;
};

class Cubemap : public Buffer
{
public:
	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
	VkDeviceSize imageSize;
	int descriptorIndex = -1;

	Cubemap(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, CubemapFaceData faces);
	~Cubemap();
	void createCubemapBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, CubemapFaceData faces);
	void dispose(VkDevice device);
	
};

