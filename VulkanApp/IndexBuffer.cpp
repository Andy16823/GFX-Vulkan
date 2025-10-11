#include "IndexBuffer.h"

VkBuffer IndexBuffer::createIndexBuffer(std::vector<uint32_t>* indices, VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

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

	// 2. Copy index data to the staging buffer
	void* data;
	vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices->data(), (size_t)bufferSize);
	vkUnmapMemory(m_device, stagingBufferMemory);

	// 3. Create the index buffer with device local memory
	VkBuffer buffer;
	createBuffer(m_physicalDevice,
		m_device,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&buffer,
		&m_indexBufferMemory);

	// 4. Copy the data from the staging buffer to the index buffer using command buffer
	copyBuffer(m_device, transferQueue, transferCommandPool, stagingBuffer, buffer, bufferSize);

	// 5. Clean up the staging buffer
	vkDestroyBuffer(m_device, stagingBuffer, nullptr);
	vkFreeMemory(m_device, stagingBufferMemory, nullptr);

	return buffer;
}

IndexBuffer::IndexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices)
{
	m_indexCount = indices->size();
	m_physicalDevice = newPhysicalDevice;
	m_device = newDevice;
	m_indexBuffer = createIndexBuffer(indices, transferQueue, transferCommandPool);
}

IndexBuffer::~IndexBuffer()
{

}

int IndexBuffer::getIndexCount()
{
	return m_indexCount;
}

VkBuffer IndexBuffer::getIndexBuffer()
{
	return m_indexBuffer;
}

void IndexBuffer::dispose()
{
	vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
	vkFreeMemory(m_device, m_indexBufferMemory, nullptr);
}
