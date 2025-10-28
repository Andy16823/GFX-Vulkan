#include "FontTextureBuffer.h"
#include "../Utils.h"

void FontTextureBuffer::createImageView(VkDevice device)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = VK_FORMAT_R8_UNORM;
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

FontTextureBuffer::FontTextureBuffer(const FontAtlas& fontAtlas, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool)
{
	imageSize = fontAtlas.atlasWidth * fontAtlas.atlasHeight;

	// Create Staging Buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(
		physicalDevice,
		device,
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, fontAtlas.pixelData.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	// Create Image
	image = createImage(
		physicalDevice,
		device,
		fontAtlas.atlasWidth,
		fontAtlas.atlasHeight,
		VK_FORMAT_R8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&imageMemory);

	// Transition Image Layout and Copy Buffer to Image
	transitionImageLayout(
		device,
		queue,
		pool,
		image,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	copyImageBuffer(
		device,
		queue,
		pool,
		stagingBuffer,
		image,
		fontAtlas.atlasWidth,
		fontAtlas.atlasHeight
	);

	transitionImageLayout(
		device,
		queue,
		pool,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	);

	// Clean up staging buffer
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// Create Image View
	createImageView(device);
}

void FontTextureBuffer::dispose(VkDevice device)
{
	vkDestroyImageView(device, imageView, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, imageMemory, nullptr);
}
