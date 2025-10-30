#pragma once
#include "RenderPass.h"
class OffscreenRenderPass : public RenderPass
{
private:
	VkRenderPass m_renderPass;
public:
	void createRenderPass(VkDevice device, VkFormat swapChainImageFormat, VkFormat depthFormat) override;
	void dispose(VkDevice device) override;
	VkRenderPass getRenderPass() override { return m_renderPass; }
};

