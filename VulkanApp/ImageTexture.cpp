#include "ImageTexture.h"
#include "stb_image.h"
#include "Renderer.h"
#include <iostream>
#include "Utils.h"

void ImageTexture::loadImageData(std::string filename)
{
	this->imageData = loadTextureFile(filename, &width, &height, &imageSize);
}

ImageTexture::ImageTexture(std::string filename) : Buffer()
{
	loadImageData(filename);
}

void ImageTexture::init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool)
{
	// Ensure image data is loaded
	if (imageData == nullptr)
	{
		throw std::runtime_error("Image data not loaded. Call loadImageData() first.");
	}

	// Ensure texture is not already created or disposed
	if (this->state != GFX_BUFFER_STATE_NONE)
	{
		throw std::runtime_error("ImageTexture already created or disposed.");
	}

	// Create staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(
		physicalDevice,
		device,
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer,
		&stagingBufferMemory
	);

	// Copy image data to staging buffer
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imageData, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);
	stbi_image_free(imageData);

	// Create image
	image = createImage(
		physicalDevice,
		device,
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height),
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&imageMemory);

	// Transition image to be transfer destination
	transitionImageLayout(
		device,
		queue,
		pool,
		image,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	);

	// Copy data to image
	copyImageBuffer(
		device,
		queue,
		pool,
		stagingBuffer,
		image,
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	);

	// Transition image to shader readable
	transitionImageLayout(
		device,
		queue,
		pool,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	);

	// Free staging buffer
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// Create image view
	imageView = createImageView(device, image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

void ImageTexture::dispose(VkDevice device)
{
	vkDestroyImageView(device, imageView, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, imageMemory, nullptr);

	this->state = GFX_BUFFER_STATE_DISPOSED;
}
