#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderPass.h"


/// <summary>
/// Default render pass implementation
/// </summary>
class DefaultRenderPass : public RenderPass
{
private:
	VkRenderPass m_renderPass;
public:

	void createRenderPass(VkDevice device, VkFormat swapChainImageFormat, VkFormat depthFormat) override;
	void dispose(VkDevice device) override;
	VkRenderPass getRenderPass() override { return m_renderPass; }
};

