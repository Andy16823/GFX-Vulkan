#include "IndexBuffer.h"

VkBuffer IndexBuffer::createIndexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, std::vector<uint32_t>* indices, VkQueue transferQueue, VkCommandPool transferCommandPool)
{
	// Check if the buffer is already created or disposed
	if (this->state != GFX_BUFFER_STATE_NONE)
	{
		throw std::runtime_error("Index buffer already created or disposed.");
	}

	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

	// 1. Create a staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(newPhysicalDevice,
		newDevice,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory);

	// 2. Copy index data to the staging buffer
	void* data;
	vkMapMemory(newDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices->data(), (size_t)bufferSize);
	vkUnmapMemory(newDevice, stagingBufferMemory);

	// 3. Create the index buffer with device local memory
	VkBuffer buffer;
	createBuffer(newPhysicalDevice,
		newDevice,
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&buffer,
		&m_indexBufferMemory);

	// 4. Copy the data from the staging buffer to the index buffer using command buffer
	copyBuffer(newDevice, transferQueue, transferCommandPool, stagingBuffer, buffer, bufferSize);

	// 5. Clean up the staging buffer
	vkDestroyBuffer(newDevice, stagingBuffer, nullptr);
	vkFreeMemory(newDevice, stagingBufferMemory, nullptr);

	// 6. Set the state to initialized
	this->state = GFX_BUFFER_STATE_INITIALIZED;

	return buffer;
}

IndexBuffer::IndexBuffer(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices)
{
	m_indexCount = indices->size();
	m_indexBuffer = createIndexBuffer(newPhysicalDevice, newDevice, indices, transferQueue, transferCommandPool);
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

void IndexBuffer::dispose(VkDevice device)
{
	vkDestroyBuffer(device, m_indexBuffer, nullptr);
	vkFreeMemory(device, m_indexBufferMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}
