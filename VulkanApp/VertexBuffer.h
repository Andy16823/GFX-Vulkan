#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "Utils.h"

/// <summary>
/// The vertex data structure
/// </summary>
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
};

/// <summary>
/// Vertex buffer class
/// </summary>
class VertexBuffer
{
private:
	int m_vertexCount;
	VkBuffer m_vertexBuffer;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer createVertexBuffer(std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool);
public:
	VertexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	~VertexBuffer();

	int getVertexCount();
	VkBuffer getVertexBuffer();
	void dispose();
};

