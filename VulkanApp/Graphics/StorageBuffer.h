#pragma once
#include "Buffer.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

/// <summary>
/// Storage Buffer
/// Persistent memory mapped buffer that can be read and written by both the GPU and CPU
/// </summary>
class StorageBuffer : public Buffer
{
private:
	void* m_mappedMemory = nullptr;

public:
	/// <summary>
	/// The descriptor index in the descriptor set layout
	/// </summary>
	int descriptorIndex = -1;

	/// <summary>
	/// Creates a storage buffer of a specific size
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="size"></param>
	StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size);
	~StorageBuffer() = default;

	/// <summary>
	/// The Vulkan buffer handle
	/// </summary>
	VkBuffer buffer;

	/// <summary>
	/// The Vulkan device memory handle
	/// </summary>
	VkDeviceMemory bufferMemory;

	/// <summary>
	/// The size of the buffer in bytes
	/// </summary>
	uint32_t bufferSize;

	/// <summary>
	/// Updates the buffer with new data
	/// </summary>
	/// <param name="device"></param>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="offset"></param>
	void updateBuffer(VkDevice device, const void* data, VkDeviceSize size, VkDeviceSize offset = 0);

	/// <summary>
	/// Disposes the buffer and its memory
	/// </summary>
	/// <param name="device"></param>
	void dispose(VkDevice device);
};

