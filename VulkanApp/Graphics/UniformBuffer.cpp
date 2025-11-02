#include "UniformBuffer.h"
#include "../Utils.h"
#include <stdexcept>

void UniformBuffer::createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize)
{
	// Check if the buffer is already created or disposed
	if (this->state != GFX_BUFFER_STATE_NONE)
	{
		throw std::runtime_error("Uniform buffer already created or disposed.");
	}

	// Create the VkBuffer for the uniform buffer
	createBuffer(physicalDevice,
		device,
		bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&m_uniformBuffer,
		&m_uniformBufferMemory);

	// Map the buffer memory
	VkResult result = vkMapMemory(device, m_uniformBufferMemory, 0, bufferSize, 0, &m_mappedMemory);

	// Check for mapping errors
	if (result != VK_SUCCESS) {
		vkDestroyBuffer(device, m_uniformBuffer, nullptr);
		vkFreeMemory(device, m_uniformBufferMemory, nullptr);
		m_mappedMemory = nullptr;
		throw std::runtime_error("Failed to map uniform buffer memory.");
	}

	// Set the state to initialized
	this->state = GFX_BUFFER_STATE_INITIALIZED;
}

UniformBuffer::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize)
{
	m_bufferSize = bufferSize;
	createUniformBuffer(physicalDevice, device ,bufferSize);
}

UniformBuffer::~UniformBuffer()
{

}

void UniformBuffer::updateBufferData(VkDevice device, void* data, VkDeviceSize size, VkDeviceSize offset /*= 0*/)
{
	// Validate the buffer state and size
	if (this->state != GFX_BUFFER_STATE_INITIALIZED) {
		throw std::runtime_error("Uniform buffer not initialized.");
	}

	// Validate the size
	if (size + offset > m_bufferSize) {
		throw std::runtime_error("Uniform buffer overflow.");
	}

	// Copy the memory
	char* mappedData = static_cast<char*>(m_mappedMemory) + offset;
	memcpy(mappedData, data, static_cast<size_t>(size));
}

void UniformBuffer::dispose(VkDevice device)
{
	vkUnmapMemory(device, m_uniformBufferMemory);
	m_mappedMemory = nullptr;
	vkDestroyBuffer(device, m_uniformBuffer, nullptr);
	vkFreeMemory(device, m_uniformBufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}
