#pragma once
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "stb_image.h"

/// <summary>
/// Maximum number of objects
/// </summary>
const int MAX_OBJECTS = 1000;

/// <summary>
/// Required device extensions
/// </summary>
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

/// <summary>
/// Indies (location) of queue families
/// </summary>
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentationFamily = -1;

	bool isValid() {
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}

	bool gpShared() {
		return graphicsFamily == presentationFamily;
	}
};

/// <summary>
/// Swap chain support details
/// </summary>
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	bool isValid() {
		return !formats.empty() && !presentModes.empty();
	}
};

struct SwapChainImage {
	VkImage image;
	VkImageView imageView;
};

struct ShaderSourceCollection {
	std::string vert;
	std::string frag;
	std::string entryPoint = "main";
};

struct Model {
	glm::mat4 model;
};

struct UboViewProjection {
	glm::mat4 projection;
	glm::mat4 view;
};

static std::vector<char> readFile(const std::string& filename) 
{
	std::ifstream file;
	file.open(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	// Get the properties of the physical device memory
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
}

static VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = viewType;
	createInfo.format = format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) == VK_SUCCESS) {
		return imageView;
	}
	else {
		throw std::runtime_error("failed to create texture image view!");
	}
}

static void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags bufferProperties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
	// Create buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
	bufferInfo.usage = bufferUsage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}

	// Allocate memory
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, bufferProperties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	// Bind memory to buffer
	vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}

static VkImage createImage(VkPhysicalDevice physicalDevice, VkDevice device,uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory)
{
	// CREATE IMAGE
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = useFlags;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;
	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	// ALLOCATE MEMORY
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, propFlags);

	if (vkAllocateMemory(device, &allocInfo, nullptr, imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	// BIND MEMORY TO IMAGE
	vkBindImageMemory(device, image, *imageMemory, 0);

	return image;
}

static VkCommandBuffer beginCommandBuffer(VkDevice device, VkCommandPool commandPool) {
	// 1. Allocate a command buffer from the command pool
	VkCommandBuffer commandBuffer;

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate transfer command buffer!");
	}

	// 2. Begin recording the command buffer setup for one time submission
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording transfer command buffer!");
	}

	return commandBuffer;
}

static void submitCommandBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkCommandBuffer commandBuffer) {
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record transfer command buffer!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit transfer command buffer!");
	}
	vkQueueWaitIdle(queue);
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

static void copyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize) {
	//1 . Create a command buffer for the copy operation
	VkCommandBuffer transferCommandBuffer = beginCommandBuffer(device, transferCommandPool);
	
	// 2. Copy from the source buffer to the destination buffer
	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = bufferSize;
	vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	// 3. End the command buffer and submit it to the transfer queue and wait until it finishes and free the command buffer
	submitCommandBuffer(device, transferCommandPool, transferQueue, transferCommandBuffer);
}

static void copyImageBuffer(VkDevice device, VkQueue queue, VkCommandPool transferCommandPool, VkBuffer srcBuffer, VkImage destImage, uint32_t width, uint32_t height) {

	//1 . Create a command buffer for the copy operation
	VkCommandBuffer transferCommandBuffer = beginCommandBuffer(device, transferCommandPool);

	// 2. Copy from the source buffer to the destination image
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };
	vkCmdCopyBufferToImage(transferCommandBuffer, srcBuffer, destImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	// 3. End the command buffer and submit it to the transfer queue and wait until it finishes and free the command buffer
	submitCommandBuffer(device, transferCommandPool, queue, transferCommandBuffer);
}

static void transitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image, VkImageLayout srcLayout, VkImageLayout destLayout) {

	VkCommandBuffer commandBuffer = beginCommandBuffer(device, commandPool);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = srcLayout;										// Layout to transition from
	barrier.newLayout = destLayout;										// Layout to transition to		
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;				// Queue family to transition from
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;				// Queue family to transition to
	barrier.image = image;												// Image beeing accessed and modified as part of the barrier
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;	// Which aspect of the image is included in the barrier (e.g. color, depth, stencil)
	barrier.subresourceRange.baseMipLevel = 0;							// First mip level to be included in the barrier
	barrier.subresourceRange.levelCount = 1;							// Number of mip levels to be included in the barrier
	barrier.subresourceRange.baseArrayLayer = 0;						// First array layer to be included in the barrier
	barrier.subresourceRange.layerCount = 1;							// Number of array layers to be included in the barrier

	VkPipelineStageFlags srcStage;
	VkPipelineStageFlags dstStage;

	// if transition from new image to image to recive data ..
	if (srcLayout == VK_IMAGE_LAYOUT_UNDEFINED && destLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;										// Dont need to wait on anything
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;			// must happen before transfer write

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	// if transition from image that has recived data to image to be read by shader ..
	else if (srcLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && destLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;			// Wait after transfer write
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;				// must happen before shader read

		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		srcStage, dstStage,		// Pipeline stages
		0,						// Dependency flags
		0, nullptr,				// Memory Barrier count + data
		0, nullptr,				// Buffer Memory Barrier count + data
		1, &barrier				// Image Memory Barrier count and data
	);

	submitCommandBuffer(device, commandPool, queue, commandBuffer);
}

static stbi_uc* loadTextureFile(std::string fileName, int* width, int* height)
{
	int channels;
	stbi_uc* image = stbi_load(fileName.c_str(), width, height, &channels, STBI_rgb_alpha);
	if (!image) {
		throw std::runtime_error("failed to load texture image!");
	}
	return image;
}