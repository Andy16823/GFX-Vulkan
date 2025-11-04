#include "Scene.h"

void Scene::init(Renderer* renderer)
{
	// Create a render target for this scene
	m_renderTargetIndex = renderer->createRenderTarget(true);
}

void Scene::beforeSwapchainRecreate(Renderer* renderer)
{
	// Clean up the existing render target
	renderer->cleanupRenderTarget(m_renderTargetIndex);
}

void Scene::afterSwapchainRecreate(Renderer* renderer, const glm::ivec2& newSize)
{
	// Recreate the render target with the new swapchain size
	renderer->recreateRenderTarget(m_renderTargetIndex);
}
