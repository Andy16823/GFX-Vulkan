#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "../Utils.h"

/// <summary>
/// The vertex data structure
/// </summary>
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

/// <summary>
/// Vertex buffer class
/// </summary>
class VertexBuffer : public Buffer
{
private:
	int m_vertexCount;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool);
public:
	VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	~VertexBuffer();

	int getVertexCount();
	VkBuffer getVertexBuffer();
	void dispose(VkDevice device);
};

