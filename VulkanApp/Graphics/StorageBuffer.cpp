#include "StorageBuffer.h"
#include "../Utils.h"
#include <iostream>

StorageBuffer::StorageBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, bool persistent)
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

	if (persistent) {
		// Map the buffer memory persistently
		vkMapMemory(device, bufferMemory, 0, size, 0, &m_mappedMemory);
		m_persistentlyMapped = true;
		std::cout << "[STORAGE BUFFER] Storage buffer persistently mapped." << std::endl;
	}
	else {
		std::cout << "[STORAGE BUFFER] Storage buffer created with size: "
			<< bufferSize << " bytes." << std::endl;
	}
}

void StorageBuffer::updateBuffer(VkDevice device, const void* data, VkDeviceSize size, VkDeviceSize offset /*= 0*/)
{
	// Check if the data size exceeds the buffer size
	VkDeviceSize dataSize = size;
	if (offset + dataSize > bufferSize) {
		throw std::runtime_error("Data size exceeds storage buffer capacity.");
	}

	// If persistently mapped, use the mapped memory
	if (m_persistentlyMapped) {
		char* mappedData = static_cast<char*>(m_mappedMemory) + offset;
		memcpy(mappedData, data, static_cast<size_t>(dataSize));
		return;
	}

	// Map the buffer memory and copy the data
	void* mappedData;
	vkMapMemory(device, bufferMemory, offset, dataSize, 0, &mappedData);
	memcpy(mappedData, data, static_cast<size_t>(dataSize));
	vkUnmapMemory(device, bufferMemory);
}

void StorageBuffer::dispose(VkDevice device)
{
	// Unmap memory if persistently mapped
	if (m_persistentlyMapped) {
		vkUnmapMemory(device, bufferMemory);
		m_persistentlyMapped = false;
		m_mappedMemory = nullptr;
	}

	// Destroy the buffer and free memory
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, bufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
	std::cout << "[STORAGE BUFFER] Storage buffer disposed." << std::endl;
}

