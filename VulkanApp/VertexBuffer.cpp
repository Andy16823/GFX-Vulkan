#include "VertexBuffer.h"

VkBuffer VertexBuffer::createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool)
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

	return buffer;
}

VertexBuffer::VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices)
{
	m_vertexCount = vertices->size();
	m_vertexBuffer = createVertexBuffer(physicalDevice, device,vertices, transferQueue, transferCommandPool);
}

VertexBuffer::~VertexBuffer()
{
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
	vkDestroyBuffer(device, m_vertexBuffer, nullptr);
	vkFreeMemory(device, m_vertexBufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}
