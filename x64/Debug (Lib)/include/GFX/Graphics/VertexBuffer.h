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
/// The vertex buffer types
/// </summary>
enum class VertexBufferType {
	VERTEX_BUFFER_TYPE_STATIC,
	VERTEX_BUFFER_TYPE_DYNAMIC
};

/// <summary>
/// Vertex buffer class
/// Type: Static
/// Store vertex data in a device local buffer. Data cannot be changed after creation.
/// Type: Dynamic
/// Store vertex data in a host visible buffer. Data can be updated at any time.
/// Dynamic buffers are allways persistently mapped for best performance.
/// </summary>
class VertexBuffer : public Buffer
{
private:
	/// <summary>
	/// Vertex count in the buffer
	/// </summary>
	int m_vertexCount;

	/// <summary>
	/// Capacity of the buffer in number of vertices (for dynamic buffers only)
	/// </summary>
	uint32_t m_capacity;

	/// <summary>
	/// The Vulkan vertex buffer handle
	/// </summary>
	VkBuffer m_vertexBuffer;

	/// <summary>
	/// The Vulkan vertex buffer memory handle
	/// </summary>
	VkDeviceMemory m_vertexBufferMemory;

	/// <summary>
	/// Pristently mapped memory pointer for dynamic buffers
	/// </summary>
	void* m_mappedMemory = nullptr;

	/// <summary>
	/// Resize the dynamic vertex buffer to fit the new vertex count
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="vertices"></param>
	void resizeBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);

	/// <summary>
	/// Create a static vertex buffer
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="vertices"></param>
	/// <param name="transferQueue"></param>
	/// <param name="transferCommandPool"></param>
	/// <returns></returns>
	VkBuffer createStaticVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool);

	/// <summary>
	/// Create a dynamic vertex buffer
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="vertices"></param>
	/// <returns></returns>
	VkBuffer createDynamicVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);

public:
	/// <summary>
	/// The vertex buffer type
	/// </summary>
	VertexBufferType type;

	/// <summary>
	/// Create a new vertex buffer
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="transferQueue"></param>
	/// <param name="transferCommandPool"></param>
	/// <param name="vertices"></param>
	/// <param name="buffertype"></param>
	VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, const VertexBufferType buffertype = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	~VertexBuffer();

	/// <summary>
	/// Update the dynamic vertex buffer with new vertex data
	/// Resizes the buffer if the new data exceeds the current capacity
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="vertices"></param>
	void updateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices);

	/// <summary>
	/// Get the number of vertices in the buffer
	/// </summary>
	/// <returns></returns>
	int getVertexCount();

	/// <summary>
	/// Get the Vulkan vertex buffer handle
	/// </summary>
	/// <returns></returns>
	VkBuffer getVertexBuffer();

	/// <summary>
	/// Free the vertex buffer resources
	/// </summary>
	/// <param name="device"></param>
	void dispose(VkDevice device);
};

