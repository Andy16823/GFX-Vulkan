#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Buffer.h"
#include <GLFW/glfw3.h>

/// <summary>
/// Uniform Buffer Class
/// </summary>
class UniformBuffer : public Buffer
{
private:
	/// <summary>
	/// The VkBuffer for the uniform buffer
	/// </summary>
	VkBuffer m_uniformBuffer;

	/// <summary>
	/// The VkDeviceMemory for the uniform buffer
	/// </summary>
	VkDeviceMemory m_uniformBufferMemory;

	/// <summary>
	/// The Buffer Size
	/// </summary>
	VkDeviceSize m_bufferSize;

	/// <summary>
	/// Determines if the buffer is persistently mapped
	/// </summary>
	bool m_persistentlyMapped = false;

	/// <summary>
	/// The mapped memory pointer if persistently mapped
	/// </summary>
	void* m_mappedMemory = nullptr;

	/// <summary>
	/// Creates the uniform buffer
	/// </summary>
	/// <param name="physicalDevice"></param>
	/// <param name="device"></param>
	/// <param name="bufferSize"></param>
	void createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize, bool persistent);
public:
	UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize, bool persistent);
	~UniformBuffer();
	VkBuffer getUniformBuffer() { return m_uniformBuffer; }

	void updateBufferData(VkDevice device, void* data, VkDeviceSize size, VkDeviceSize offset = 0);
	void dispose(VkDevice device);
};

