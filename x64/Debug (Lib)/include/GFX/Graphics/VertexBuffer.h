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

enum class VertexBufferType {
	VERTEX_BUFFER_TYPE_STATIC,
	VERTEX_BUFFER_TYPE_DYNAMIC
};

/// <summary>
/// Vertex buffer class
/// </summary>
class VertexBuffer : public Buffer
{
private:
	int m_vertexCount;
	uint32_t m_capacity;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	void resizeBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);
	VkBuffer createStaticVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool);
	VkBuffer createDynamicVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);
public:
	VertexBufferType type;

	VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, const VertexBufferType buffertype = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	~VertexBuffer();

	// TODO: Check for resize and recreate buffer if needed for now we resize always
	void updateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);

	int getVertexCount();
	VkBuffer getVertexBuffer();
	void dispose(VkDevice device);
};

