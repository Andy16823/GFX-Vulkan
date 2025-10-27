#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VertexBuffer.h"

class Renderer;

/// <summary>
/// Render target class for offscreen rendering
/// </summary>
class RenderTarget 
{
private:
	VkImage m_image;						// The image used as the render target
	VkDeviceMemory m_imageMemory;			// Memory for the render target image
	VkImageView m_imageView;				// Image view for the render target
	VkFramebuffer m_framebuffer;			// Framebuffer for the render target
	VkCommandBuffer m_commandBuffer;		// Command buffer for rendering to the render target

	int m_offscreenQuadVBO;					// Vertex buffer object for the fullscreen quad
	int m_offscreenQuadIBO;					// Index buffer object for the fullscreen quad
	int m_offscreenDescriptorIndex;			// Descriptor index for sampling the render target

public:
	RenderTarget() = default;
	~RenderTarget() = default;

	VkCommandBuffer getCommandBuffer();

	void createRenderTarget(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D extent, VkFormat format, VkRenderPass renderpass);
	void createOffscreenQuadBuffers(Renderer* renderer);
	void createCommandBuffer(VkDevice device, VkCommandPool commandPool);
	void dispose(VkDevice device);

	void startRecord(VkDevice device);
	void endRecord(VkDevice device);
	void submitAndWait(VkQueue graphicsQueue, VkDevice device);

	void setDescriptorIndex(int index) { m_offscreenDescriptorIndex = index; }

	VkImageView getImageView() const { return m_imageView; }
	VkFramebuffer getFramebuffer() const { return m_framebuffer; }
	int getOffscreenQuadVBO() const { return m_offscreenQuadVBO; }
	int getOffscreenQuadIBO() const { return m_offscreenQuadIBO; }
	int getOffscreenDescriptorIndex() const { return m_offscreenDescriptorIndex; }
	static std::vector<Vertex> getFullscreenQuadVertices();
	static std::vector<uint32_t> getFullscreenQuadIndices();
};

