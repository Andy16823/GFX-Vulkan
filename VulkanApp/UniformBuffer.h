#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



class UniformBuffer
{
private:
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkBuffer m_uniformBuffer;
	VkDeviceMemory m_uniformBufferMemory;
	VkDeviceSize m_bufferSize;
	void createUniformBuffer(VkDeviceSize bufferSize);
public:
	UniformBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkDeviceSize bufferSize);
	~UniformBuffer();
	VkBuffer getUniformBuffer();
	VkDeviceSize getBufferSize() { return m_bufferSize; };
	VkDeviceMemory getUniformBufferMemory() { return m_uniformBufferMemory; };
	void dispose();
};

