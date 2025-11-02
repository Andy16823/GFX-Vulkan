#pragma once
#include "Buffer.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class StorageBuffer : public Buffer
{
private:
	/// <summary>
	/// Determines if the buffer is persistently mapped
	/// </summary>
	bool m_persistentlyMapped = false;
	void* m_mappedMemory = nullptr;

public:
	int descriptorIndex = -1;

	StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, bool persistent = false);
	~StorageBuffer() = default;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	uint32_t bufferSize;
	void updateBuffer(VkDevice device, const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
	void dispose(VkDevice device);
};

