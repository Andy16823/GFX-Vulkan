#include "ImageBuffer.h"
#include "../stb_image.h"
#include "Renderer.h"
#include <iostream>
#include "../Utils.h"

void ImageBuffer::calcImageSize(int width, int height, ImageBufferFormat format)
{
	switch (format)
	{
	case ImageBufferFormat::RGBA:
		this->imageSize = width * height * 4;
		break;
	case ImageBufferFormat::RGB:
		this->imageSize = width * height * 3;
		break;
	case ImageBufferFormat::GRAY:
		this->imageSize = width * height * 1;
		break;
	default:
		throw std::runtime_error("Unsupported image buffer format.");
	}
}

void ImageBuffer::createImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool)
{
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

VkFormat ImageBuffer::getVkFormat(ImageBufferFormat format)
{
	switch (format)
	{
	case ImageBufferFormat::RGBA:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case ImageBufferFormat::RGB:
		return VK_FORMAT_R8G8B8_UNORM;
	case ImageBufferFormat::GRAY:
		return VK_FORMAT_R8_UNORM;
	default:
		throw std::runtime_error("Unsupported image buffer format.");
	}
}

ImageBuffer::ImageBuffer(stbi_uc* imageData, int width, int height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool)
{
	this->calcImageSize(width, height, ImageBufferFormat::RGBA);
	//this->imageSize = width * height * 4; // Force 4 channels (RGBA)
	createImageBuffer(imageData, width, height, physicalDevice, device, queue, pool);
}

ImageBuffer::ImageBuffer(std::vector<unsigned char> imageData, uint32_t width, uint32_t height, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, ImageBufferFormat format)
{
	// Calculate image size based on format
	this->calcImageSize(width, height, format);

	// Get the target format
	VkFormat targetFormat = getVkFormat(format);

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
	memcpy(data, imageData.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	// Create Image
	image = createImage(
		physicalDevice,
		device,
		width,
		height,
		targetFormat,
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
		width,
		height
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

	// Create image view
	imageView = createImageView(device, image, targetFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void ImageBuffer::dispose(VkDevice device)
{
	vkDestroyImageView(device, imageView, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, imageMemory, nullptr);
	this->state = GFX_BUFFER_STATE_DISPOSED;
}