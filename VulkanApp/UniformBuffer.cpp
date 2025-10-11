#include "UniformBuffer.h"
#include "Utils.h"

void UniformBuffer::createUniformBuffer(VkDeviceSize bufferSize)
{
	createBuffer(m_physicalDevice,
		m_device,
		bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&m_uniformBuffer,
		&m_uniformBufferMemory);
}

UniformBuffer::UniformBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkDeviceSize bufferSize)
{
	m_physicalDevice = newPhysicalDevice;
	m_device = newDevice;
	m_bufferSize = bufferSize;
	createUniformBuffer(bufferSize);
}

UniformBuffer::~UniformBuffer()
{

}

VkBuffer UniformBuffer::getUniformBuffer()
{
	return m_uniformBuffer;
}

void UniformBuffer::dispose()
{
	vkDestroyBuffer(m_device, m_uniformBuffer, nullptr);
	vkFreeMemory(m_device, m_uniformBufferMemory, nullptr);
}
