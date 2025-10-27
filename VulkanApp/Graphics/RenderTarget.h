#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VertexBuffer.h"

class Renderer;

/// <summary>
/// Represents an offscreen render target, including the Vulkan image,
/// framebuffer, and associated resources for rendering to a texture.
/// </summary>
class RenderTarget
{
private:
    /// <summary>
    /// Vulkan image used as the offscreen render target.
    /// </summary>
    VkImage m_image;

    /// <summary>
    /// Memory allocated for the render target image.
    /// </summary>
    VkDeviceMemory m_imageMemory;

    /// <summary>
    /// Image view for accessing the render target image.
    /// </summary>
    VkImageView m_imageView;

    /// <summary>
    /// Framebuffer object linked to the render target.
    /// </summary>
    VkFramebuffer m_framebuffer;

    /// <summary>
    /// Command buffer used for recording rendering commands to this render target.
    /// </summary>
    VkCommandBuffer m_commandBuffer;

    /// <summary>
    /// Vertex buffer object for the fullscreen quad used in post-processing.
    /// </summary>
    int m_offscreenQuadVBO;

    /// <summary>
    /// Index buffer object for the fullscreen quad.
    /// </summary>
    int m_offscreenQuadIBO;

    /// <summary>
    /// Descriptor index used for sampling this render target in shaders.
    /// </summary>
    int m_offscreenDescriptorIndex;

	/// <summary>
	/// Determines if the render target is intended to be presented on screen.
	/// </summary>
	bool m_presentOnScreen = false;

    /// <summary>
	/// The depth image for the render target.
    /// </summary>
    VkImage m_depthImage;

	/// <summary>
	/// The memory allocated for the depth image.
	/// </summary>
	VkDeviceMemory m_depthImageMemory;

	/// <summary>
	/// The image view for the depth image.
	/// </summary>
	VkImageView m_depthImageView;

public:
    RenderTarget(const bool presentOnScreen);
    ~RenderTarget() = default;

    /// <summary>
    /// Returns the command buffer used for recording rendering commands.
    /// </summary>
    /// <returns>The Vulkan command buffer associated with this render target.</returns>
    VkCommandBuffer getCommandBuffer();

    /// <summary>
    /// Creates the Vulkan image, memory, and framebuffer for the render target.
    /// </summary>
    /// <param name="physicalDevice">The Vulkan physical device used for memory allocation.</param>
    /// <param name="device">The Vulkan logical device used for resource creation.</param>
    /// <param name="extent">The dimensions of the render target.</param>
    /// <param name="format">The image format of the render target.</param>
    /// <param name="renderpass">The render pass used to create the framebuffer.</param>
    void createRenderTarget(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D extent, VkFormat format, VkRenderPass renderpass, VkFormat depthFormat);

    /// <summary>
    /// Creates vertex and index buffers for the fullscreen quad.
    /// </summary>
    /// <param name="renderer">Pointer to the renderer instance used to create GPU buffers.</param>
    void createOffscreenQuadBuffers(Renderer* renderer);

    /// <summary>
    /// Creates the command buffer used for rendering to the offscreen target.
    /// </summary>
    /// <param name="device">The Vulkan logical device.</param>
    /// <param name="commandPool">The command pool used to allocate the command buffer.</param>
    void createCommandBuffer(VkDevice device, VkCommandPool commandPool);

    /// <summary>
    /// Releases all Vulkan resources associated with this render target.
    /// </summary>
    /// <param name="device">The Vulkan logical device used to destroy resources.</param>
    void dispose(VkDevice device);

    /// <summary>
    /// Begins command buffer recording for rendering to this render target.
	/// Only use this if you want record statically. Otherwise, use the renderer's command buffers within the 
    /// onRenderCallback.
    /// </summary>
    /// <param name="device">The Vulkan logical device.</param>
    void startRecord(VkDevice device);

    /// <summary>
    /// Ends command buffer recording.
	/// Only use this if you want record statically. Otherwise, use the renderer's command buffers within the
	/// onRenderCallback.
    /// </summary>
    /// <param name="device">The Vulkan logical device.</param>
    void endRecord(VkDevice device);

    /// <summary>
    /// Submits the recorded command buffer for execution on the given graphics queue.
	/// Only use this if you want to submit statically. Otherwise, use the renderer's command buffers within the
	/// onRenderCallback.
    /// </summary>
    /// <param name="graphicsQueue">The Vulkan graphics queue to submit to.</param>
    /// <param name="device">The Vulkan logical device.</param>
    void submit(VkQueue graphicsQueue, VkDevice device);

    /// <summary>
    /// Submits the command buffer and waits until the operation completes.
	/// Only use this if you want to submit statically. Otherwise, use the renderer's command buffers within the
	/// onRenderCallback.
	/// Very expensive operation, avoid if possible in real-time rendering.
    /// </summary>
    /// <param name="graphicsQueue">The Vulkan graphics queue to submit to.</param>
    /// <param name="device">The Vulkan logical device.</param>
    void submitAndWait(VkQueue graphicsQueue, VkDevice device);

    /// <summary>
    /// Sets the descriptor index used for this render target when binding in shaders.
    /// </summary>
    /// <param name="index">Descriptor index.</param>
    void setDescriptorIndex(int index) { m_offscreenDescriptorIndex = index; }

    /// <summary>
    /// Gets the Vulkan image view for this render target.
    /// </summary>
    /// <returns>The image view associated with this render target.</returns>
    VkImageView getImageView() const { return m_imageView; }

    /// <summary>
    /// Gets the framebuffer handle used by this render target.
    /// </summary>
    /// <returns>The Vulkan framebuffer handle.</returns>
    VkFramebuffer getFramebuffer() const { return m_framebuffer; }

    /// <summary>
    /// Gets the vertex buffer object handle for the fullscreen quad.
    /// </summary>
    /// <returns>The VBO handle.</returns>
    int getOffscreenQuadVBO() const { return m_offscreenQuadVBO; }

    /// <summary>
    /// Gets the index buffer object handle for the fullscreen quad.
    /// </summary>
    /// <returns>The IBO handle.</returns>
    int getOffscreenQuadIBO() const { return m_offscreenQuadIBO; }

    /// <summary>
    /// Gets the descriptor index used for shader sampling.
    /// </summary>
    /// <returns>The descriptor index.</returns>
    int getOffscreenDescriptorIndex() const { return m_offscreenDescriptorIndex; }

    /// <summary>
    /// Returns a set of vertices defining a fullscreen quad.
    /// </summary>
    /// <returns>A list of vertices for the fullscreen quad.</returns>
    static std::vector<Vertex> getFullscreenQuadVertices();

    /// <summary>
    /// Returns the index list for rendering the fullscreen quad.
    /// </summary>
    /// <returns>A list of 32-bit indices.</returns>
    static std::vector<uint32_t> getFullscreenQuadIndices();

	/// <summary>
	/// Defines whether this render target is intended to be presented on screen.
	/// </summary>
	/// <returns></returns>
	bool isPresentOnScreen() const { return m_presentOnScreen; }
};
