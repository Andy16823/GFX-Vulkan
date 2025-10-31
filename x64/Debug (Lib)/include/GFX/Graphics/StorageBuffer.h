#pragma once
#include "Buffer.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class StorageBuffer : public Buffer
{
public:
	int descriptorIndex = -1;

	StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size);
	~StorageBuffer() = default;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	uint32_t bufferSize;
	void updateBuffer(VkDevice device, const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
	void dispose(VkDevice device);
};

