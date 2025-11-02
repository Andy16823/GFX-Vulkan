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

	// Get the buffer size
	bufferSize = static_cast<uint32_t>(size);

	// Map the buffer memory for persistent access
	VkResult result = vkMapMemory(device, bufferMemory, 0, size, 0, &m_mappedMemory);

	// Check for mapping errors
	if (result != VK_SUCCESS) {
		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
		m_mappedMemory = nullptr;
		throw std::runtime_error("Failed to map storage buffer memory.");
	}

	// Debug output
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

	// Copy the data to the mapped memory
	char* mappedData = static_cast<char*>(m_mappedMemory) + offset;
	memcpy(mappedData, data, static_cast<size_t>(dataSize));
}

void StorageBuffer::dispose(VkDevice device)
{
	// Unmap memory
	vkUnmapMemory(device, bufferMemory);
	m_mappedMemory = nullptr;

	// Destroy the buffer and free memory
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, bufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
	std::cout << "[STORAGE BUFFER] Storage buffer disposed." << std::endl;
}