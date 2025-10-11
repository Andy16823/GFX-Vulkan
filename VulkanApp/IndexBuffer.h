#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "Utils.h"

class IndexBuffer
{
private:
	int m_indexCount;
	VkBuffer m_indexBuffer;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkDeviceMemory m_indexBufferMemory;

	VkBuffer createIndexBuffer(std::vector<uint32_t>* indices, VkQueue transferQueue, VkCommandPool transferCommandPool);
public:
	IndexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);
	~IndexBuffer();
	int getIndexCount();
	VkBuffer getIndexBuffer();
	void dispose();
};

