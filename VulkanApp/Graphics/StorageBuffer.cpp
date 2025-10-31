#include "StorageBuffer.h"
#include "../Utils.h"
#include <iostream>

StorageBuffer::StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size)
{
	// Create the storage buffer
	createBuffer(
		physicalDevice,
		device,
		size,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&buffer,
		&bufferMemory);

	bufferSize = static_cast<uint32_t>(size);

	std::cout << "[STORAGE BUFFER] Storage buffer created with size: "
		<< bufferSize << " bytes." << std::endl;
}

void StorageBuffer::updateBuffer(VkDevice device, const void* data, VkDeviceSize size, VkDeviceSize offset /*= 0*/)
{
	// Check if the data size exceeds the buffer size
	VkDeviceSize dataSize = size;
	if (offset + dataSize > bufferSize) {
		throw std::runtime_error("Data size exceeds storage buffer capacity.");
	}

	// Map the buffer memory and copy the data
	void* mappedData;
	vkMapMemory(device, bufferMemory, offset, dataSize, 0, &mappedData);
	memcpy(mappedData, data, static_cast<size_t>(dataSize));
	vkUnmapMemory(device, bufferMemory);
}

void StorageBuffer::dispose(VkDevice device)
{
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, bufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
	std::cout << "[STORAGE BUFFER] Storage buffer disposed." << std::endl;
}

