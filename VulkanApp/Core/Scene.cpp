#include "Scene.h"

void Scene::init(Renderer* renderer)
{
	// Create a render target for this scene
	m_renderTargetIndex = renderer->createRenderTarget(true);

	for (const auto& behavior : m_sceneBehaviors) {
		behavior->init(this, renderer);
	}
}

void Scene::update(float deltaTime)
{
	for (const auto& behavior : m_sceneBehaviors) {
		behavior->update(this, deltaTime);
	}
}

void Scene::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	for (const auto& behavior : m_sceneBehaviors) {
		behavior->render(this, renderer, commandBuffer, currentFrame);
	}
}

void Scene::beforeSwapchainRecreate(Renderer* renderer)
{
	// Clean up the existing render target
	renderer->cleanupRenderTarget(m_renderTargetIndex);
	for (const auto& behavior : m_sceneBehaviors) {
		behavior->beforeSwapchainRecreation(this, renderer);
	}
}

void Scene::afterSwapchainRecreate(Renderer* renderer, const glm::ivec2& newSize)
{
	// Recreate the render target with the new swapchain size
	renderer->recreateRenderTarget(m_renderTargetIndex);
	for (const auto& behavior : m_sceneBehaviors) {
		behavior->afterSwapchainRecreation(this, renderer, newSize);
	}
}

void Scene::destroy(Renderer* renderer)
{
	for (const auto& behavior : m_sceneBehaviors) 
	{
		behavior->destroy(this, renderer);
	}
}
