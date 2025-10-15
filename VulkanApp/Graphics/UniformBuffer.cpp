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

VkBuffer UniformBuffer::getUniformBuffer()
{
	return m_uniformBuffer;
}

void UniformBuffer::dispose(VkDevice device)
{
	vkDestroyBuffer(device, m_uniformBuffer, nullptr);
	vkFreeMemory(device, m_uniformBufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}
