#include "VertexBuffer.h"
#include <iostream>
#include "Renderer.h"

void VertexBuffer::resizeBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices)
{
	// Check if the buffer type is dynamic
	if (this->type != VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC) {
		throw std::runtime_error("Only dynamic vertex buffers can be resized.");
	}
	std::cout << "[VERTEX BUFFER] Resizing dynamic vertex buffer to capacity: " << m_capacity << " vertices." << std::endl;

	// Wait for the device to be idle before resizing
	vkDeviceWaitIdle(device);

	// Set new capacity
	m_capacity = vertices->size();

	// Unmap memory if already mapped
	if (m_mappedMemory) {
		vkUnmapMemory(device, m_vertexBufferMemory);
		m_mappedMemory = nullptr;
	}

	// Destroy the old buffer
	if (m_vertexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, m_vertexBuffer, nullptr);
		vkFreeMemory(device, m_vertexBufferMemory, nullptr);
	}

	// Create new buffer
	VkDeviceSize bufferSize = sizeof(Vertex) * m_capacity;
	createBuffer(physicalDevice,
		device,
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&m_vertexBuffer,
		&m_vertexBufferMemory);

	// Remap the buffer memory persistently
	vkMapMemory(device, m_vertexBufferMemory, 0, bufferSize, 0, &m_mappedMemory);

	// Show debug message
	std::cout << "[VERTEX BUFFER] Dynamic vertex buffer resized to " << bufferSize << " bytes." << std::endl;
}

VkBuffer VertexBuffer::createStaticVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	// Check if the buffer is already created or disposed
	if (this->state != GFX_BUFFER_STATE_NONE)
	{
		throw std::runtime_error("Vertex buffer already created or disposed.");
	}

	// Variable to hold the buffer
	VkDeviceSize bufferSize = sizeof(Vertex) * vertices->size();

	// 1. Create a staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(physicalDevice,
		device,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory);


	// 2. Copy vertex data to the staging buffer
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices->data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	// 3. Create the vertex buffer with device local memory
	VkBuffer buffer;
	createBuffer(physicalDevice,
		device,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&buffer,
		&m_vertexBufferMemory);

	// 4. Copy the data from the staging buffer to the vertex buffer using command buffer
	copyBuffer(device, transferQueue, transferCommandPool, stagingBuffer, buffer, bufferSize);

	// 5. Clean up the staging buffer
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// 6. Set the state to initialized
	this->state = GFX_BUFFER_STATE_INITIALIZED;
	std::cout << "[VERTEX BUFFER] Static vertex buffer created with size: " << bufferSize << " bytes." << std::endl;

	// Set capacity
	m_capacity = vertices->size(); // We can pass the capacity at the end since this buffer is static and won't change size

	return buffer;
}

VkBuffer VertexBuffer::createDynamicVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices)
{
	// Check if the buffer is already created or disposed
	if (this->state != GFX_BUFFER_STATE_NONE)
	{
		throw std::runtime_error("Vertex buffer already created or disposed. Try updating the buffer instead.");
	}

	// Set default buffer size to avoid frequent reallocations
	m_capacity = vertices->size();
	if (m_capacity == 0) {
		m_capacity = 1200; // Default capacity
	}

	// Create the vertex buffer with host visible memory
	VkDeviceSize bufferSize = sizeof(Vertex) * m_capacity;
	createBuffer(physicalDevice,
		device,
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&m_vertexBuffer,
		&m_vertexBufferMemory);

	// Map the buffer memory persistently
	vkMapMemory(device, m_vertexBufferMemory, 0, bufferSize, 0, &m_mappedMemory);

	// If initial vertices are provided, copy them to the buffer
	if (!vertices->empty()) {
		memcpy(m_mappedMemory, vertices->data(), sizeof(Vertex) * vertices->size());
		m_vertexCount = vertices->size();
	}

	// Set the state to initialized
	this->state = GFX_BUFFER_STATE_INITIALIZED;
	std::cout << "[VERTEX BUFFER] Dynamic vertex buffer created with capacity: "
		<< m_capacity << " vertices (" << bufferSize << " bytes)" << std::endl;

	return m_vertexBuffer;
}

VertexBuffer::VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, const VertexBufferType buffertype)
{
	m_vertexCount = vertices->size();
	this->type = buffertype;
	switch (buffertype)
	{
	case VertexBufferType::VERTEX_BUFFER_TYPE_STATIC:
		m_vertexBuffer = createStaticVertexBuffer(physicalDevice, device, vertices, transferQueue, transferCommandPool);
		break;
	case VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC:
		m_vertexBuffer = createDynamicVertexBuffer(physicalDevice, device, vertices);
		break;
	default:
		m_vertexBuffer = createStaticVertexBuffer(physicalDevice, device, vertices, transferQueue, transferCommandPool);
		break;
	}
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::updateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices)
{
	// Check if the buffer type is dynamic
	if (this->type != VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC) {
		throw std::runtime_error("Only dynamic vertex buffers can be updated. Try changing the buffer type to VERTEX_BUFFER_TYPE_DYNAMIC.");
	}

	// Check if the buffer is initialized
	if (this->state != GFX_BUFFER_STATE_INITIALIZED)
	{
		throw std::runtime_error("Vertex buffer not initialized or already disposed.");
	}

	// Check if the vertices vector is empty
	if (vertices->empty())
	{
		m_vertexCount = 0;  // Erlaube leere Updates
		return;
	}

	// Check if we need to resize the buffer
	if (vertices->size() > m_capacity)
	{
		this->resizeBuffer(physicalDevice, device, vertices);
	}

	// Copy vertex data to the buffer
	VkDeviceSize dataSize = sizeof(Vertex) * vertices->size();
	memcpy(m_mappedMemory, vertices->data(), (size_t)dataSize);

	// Update real vertex count
	m_vertexCount = vertices->size();
}

int VertexBuffer::getVertexCount()
{
	return m_vertexCount;
}

VkBuffer VertexBuffer::getVertexBuffer()
{
	return m_vertexBuffer;
}

void VertexBuffer::dispose(VkDevice device)
{
	// Unmap memory if dynamic buffer
	if (this->type == VertexBufferType::VERTEX_BUFFER_TYPE_DYNAMIC)
	{
		vkUnmapMemory(device, m_vertexBufferMemory);
		m_mappedMemory = nullptr;
	}

	// Destroy the buffer and free memory
	vkDestroyBuffer(device, m_vertexBuffer, nullptr);
	vkFreeMemory(device, m_vertexBufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}
