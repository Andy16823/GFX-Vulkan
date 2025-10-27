#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "RenderPass.h"

class RenderPassManager
{
private:
	std::vector<std::unique_ptr<RenderPass>> m_renderPasses;

public:
	RenderPassManager() = default;
	~RenderPassManager() = default;
	int addRenderPass(std::unique_ptr<RenderPass> renderPass);
	RenderPass* getRenderPass(int index);
	void dispose(VkDevice device);
};

