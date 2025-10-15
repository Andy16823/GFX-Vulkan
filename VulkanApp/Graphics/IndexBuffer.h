#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "../Utils.h"

class IndexBuffer : public Buffer
{
private:
	int m_indexCount;
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	VkBuffer createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<uint32_t>* indices, VkQueue transferQueue, VkCommandPool transferCommandPool);
public:
	IndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);
	~IndexBuffer();
	int getIndexCount();
	VkBuffer getIndexBuffer();
	void dispose(VkDevice device);
};

