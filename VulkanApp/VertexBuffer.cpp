#include "VertexBuffer.h"

VkBuffer VertexBuffer::createVertexBuffer(std::vector<Vertex>* vertices, VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	// Variable to hold the buffer
	VkDeviceSize bufferSize = sizeof(Vertex) * vertices->size();

	// 1. Create a staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(m_physicalDevice,
		m_device,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory);


	// 2. Copy vertex data to the staging buffer
	void* data;
	vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices->data(), (size_t)bufferSize);
	vkUnmapMemory(m_device, stagingBufferMemory);

	// 3. Create the vertex buffer with device local memory
	VkBuffer buffer;
	createBuffer(m_physicalDevice,
		m_device,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&buffer,
		&m_vertexBufferMemory);

	// 4. Copy the data from the staging buffer to the vertex buffer using command buffer
	copyBuffer(m_device, transferQueue, transferCommandPool, stagingBuffer, buffer, bufferSize);

	// 5. Clean up the staging buffer
	vkDestroyBuffer(m_device, stagingBuffer, nullptr);
	vkFreeMemory(m_device, stagingBufferMemory, nullptr);

	return buffer;
}

VertexBuffer::VertexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices)
{
	m_vertexCount = vertices->size();
	m_physicalDevice = newPhysicalDevice;
	m_device = newDevice;
	m_vertexBuffer = createVertexBuffer(vertices, transferQueue, transferCommandPool);
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

void VertexBuffer::dispose()
{
	vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
	vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
}
