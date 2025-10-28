#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>



class UniformBuffer : public Buffer
{
private:
	VkBuffer m_uniformBuffer;
	VkDeviceMemory m_uniformBufferMemory;
	VkDeviceSize m_bufferSize;

	void createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize);
public:
	UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize);
	~UniformBuffer();
	VkBuffer getUniformBuffer();
	VkDeviceSize getBufferSize() { return m_bufferSize; };
	VkDeviceMemory getUniformBufferMemory() { return m_uniformBufferMemory; };
	void dispose(VkDevice device);
};

