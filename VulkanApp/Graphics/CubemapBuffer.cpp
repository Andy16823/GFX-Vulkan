#include "CubemapBuffer.h"
#include <stdexcept>
#include <array>

CubemapBuffer::CubemapBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, CubemapFaceData faces)
{
	this->createCubemapBuffer(physicalDevice, device, queue, pool, faces);
}

CubemapBuffer::~CubemapBuffer()
{

}

void CubemapBuffer::createCubemapBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue queue, VkCommandPool pool, CubemapFaceData faces)
{
	// VALIDATE BUFFER STATE
	if (this->state != GFX_BUFFER_STATE_NONE) {
		throw std::runtime_error("Cubemap buffer must be in NEW state to create cubemap.");
	}

	// VALIDATE FACE DATA
	if (faces.faceData.size() != 6) {
		throw std::runtime_error("Cubemap must have exactly 6 faces.");
	}

	// CALCULATE IMAGE SIZE
	VkDeviceSize layerSize = faces.width * faces.height * 4;	// Assuming 4 bytes per pixel (RGBA)
	VkDeviceSize imageSize = layerSize * 6;						// Total size for 6 faces
	this->imageSize = imageSize;

	// CREATE STAGING BUFFER
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

	// COPY FACE DATA TO STAGING BUFFER
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	for (size_t i = 0; i < 6; i++) {
		memcpy((char*)data + layerSize * i, faces.faceData[i], static_cast<size_t>(layerSize));
	}
	vkUnmapMemory(device, stagingBufferMemory);

	// CREATE IMAGE
	image = createImageLayered(
		physicalDevice,
		device,
		faces.width,
		faces.height,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&imageMemory,
		6,
		VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
	);

	// TRANSITION IMAGE LAYOUT TO TRANSFER DST
	transitionImageLayoutLayerd(
		device, 
		queue, 
		pool, 
		image, 
		VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		6
	);

	// COPY DATA TO CUBEMAP IMAGE
	copyBufferToImageCubemap(
		device, 
		queue, 
		pool, 
		stagingBuffer, 
		image, 
		faces.width, 
		faces.height
	);

	// TRANSITION IMAGE LAYOUT TO SHADER READ
	transitionImageLayoutLayerd(
		device, 
		queue, 
		pool, 
		image, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
		6
	);

	// CLEANUP STAGING BUFFER
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	// CREATE IMAGE VIEW
	imageView = createImageViewLayered(
		device, 
		image, 
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_ASPECT_COLOR_BIT, 
		VK_IMAGE_VIEW_TYPE_CUBE, 
		6
	);
}

void CubemapBuffer::dispose(VkDevice device)
{
	vkDestroyImageView(device, imageView, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, imageMemory, nullptr);
}
