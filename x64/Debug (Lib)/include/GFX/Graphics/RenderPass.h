#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/// <summary>
/// Abstract base class for a render pass
/// </summary>
class RenderPass
{
public:
	/// <summary>
	/// Create the render pass
	/// </summary>
	/// <param name="device"></param>
	/// <param name="swapChainImageFormat"></param>
	/// <param name="depthFormat"></param>
	virtual void createRenderPass(VkDevice device, VkFormat swapChainImageFormat, VkFormat depthFormat) = 0;

	/// <summary>
	/// Get the Vulkan render pass handle
	/// </summary>
	/// <returns></returns>
	virtual VkRenderPass getRenderPass() = 0;

	/// <summary>
	/// Dispose of the render pass
	/// </summary>
	/// <param name="device"></param>
	virtual void dispose(VkDevice device) = 0;
};

