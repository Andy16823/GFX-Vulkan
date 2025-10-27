#include "RenderPassManager.h"
#include <stdexcept>

int RenderPassManager::addRenderPass(std::unique_ptr<RenderPass> renderPass)
{
	m_renderPasses.push_back(std::move(renderPass));
	return m_renderPasses.size() - 1;
}

RenderPass* RenderPassManager::getRenderPass(int index)
{
	if (index >= 0 && index < m_renderPasses.size()) {
		return m_renderPasses[index].get();
	}
	throw std::runtime_error("failed to get render pass: invalid render pass index!");
}

void RenderPassManager::dispose(VkDevice device)
{
	for (auto& renderPass : m_renderPasses) {
		renderPass->dispose(device);
	}
}
