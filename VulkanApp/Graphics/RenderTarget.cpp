#include "RenderTarget.h"
#include "../Utils.h"
#include "Renderer.h"

VkCommandBuffer RenderTarget::getCommandBuffer()
{
	return m_commandBuffer;
}

/// <summary>
/// Create the render target
/// </summary>
/// <param name="physicalDevice"></param>
/// <param name="device"></param>
/// <param name="extent"></param>
/// <param name="format"></param>
/// <param name="renderpass"></param>
void RenderTarget::createRenderTarget(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D extent, VkFormat format, VkRenderPass renderpass)
{
	// Create the image for the render target
	m_image = createImage(
		physicalDevice,
		device,
		extent.width,
		extent.height,
		format,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&m_imageMemory);

	// Create the image view for the render target
	m_imageView = createImageView(
		device, 
		m_image, 
		format, 
		VK_IMAGE_ASPECT_COLOR_BIT);

	VkImageView attachments[] = {
		m_imageView
	};

	// Create the framebuffer for the render target
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderpass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = attachments;
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create framebuffer for render target!");
	}
}

/// <summary>
/// Create the offscreen quad buffers
/// </summary>
/// <param name="renderer"></param>
void RenderTarget::createOffscreenQuadBuffers(Renderer* renderer)
{
	std::vector<Vertex> quadVertices = getFullscreenQuadVertices();
	m_offscreenQuadVBO = renderer->createVertexBuffer(&quadVertices);

	std::vector<uint32_t> quadIndices = getFullscreenQuadIndices();
	m_offscreenQuadIBO = renderer->createIndexBuffer(&quadIndices);
}

/// <summary>
/// Create the command buffer for rendering to the render target
/// </summary>
/// <param name="device"></param>
/// <param name="commandPool"></param>
void RenderTarget::createCommandBuffer(VkDevice device, VkCommandPool commandPool)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //  Primary can be submitted to a queue, secondary cannot and must be called from a primary command buffer
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(device, &allocInfo, &m_commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

/// <summary>
/// Dispose of the render target resources
/// </summary>
/// <param name="device"></param>
void RenderTarget::dispose(VkDevice device)
{
	vkDestroyFramebuffer(device, m_framebuffer, nullptr);
	vkDestroyImageView(device, m_imageView, nullptr);
	vkDestroyImage(device, m_image, nullptr);
	vkFreeMemory(device, m_imageMemory, nullptr);
}

/// <summary>
/// Start recording commands to the command buffer
/// </summary>
/// <param name="device"></param>
void RenderTarget::startRecord(VkDevice device)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;  // Allow the command buffer to be resubmitted while it is also already pending execution
	if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
}

/// <summary>
/// End recording commands to the command buffer
/// </summary>
/// <param name="device"></param>
void RenderTarget::endRecord(VkDevice device)
{
	if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void RenderTarget::submitAndWait(VkQueue graphicsQueue, VkDevice device)
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = 0;
	VkFence fence;
	if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create fence!");
	}
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit command buffer!");
	}
	// Wait for the fence to signal that command buffer has finished executing
	vkWaitForFences(device, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkDestroyFence(device, fence, nullptr);
}



/// <summary>
/// Returns the vertices for a fullscreen quad
/// </summary>
/// <returns></returns>
std::vector<Vertex> RenderTarget::getFullscreenQuadVertices()
{
	return {
		{{-1.0f, -1.0f, 0.0f}, {1,1,1}, {0.0f, 1.0f}, {0,0,1}},
		{{ 1.0f, -1.0f, 0.0f}, {1,1,1}, {1.0f, 1.0f}, {0,0,1}},
		{{ 1.0f,  1.0f, 0.0f}, {1,1,1}, {1.0f, 0.0f}, {0,0,1}},
		{{-1.0f,  1.0f, 0.0f}, {1,1,1}, {0.0f, 0.0f}, {0,0,1}},
	};
}

/// <summary>
/// Returns the indices for a fullscreen quad
/// </summary>
/// <returns></returns>
std::vector<uint32_t> RenderTarget::getFullscreenQuadIndices()
{
	return  { 0,1,2, 2,3,0 };
}
