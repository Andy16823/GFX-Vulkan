#include "Scene.h"

void Scene::init(Renderer* renderer)
{
	m_renderTargetIndex = renderer->createRenderTarget(true);
	if (this->hasSkybox()) {
		skybox->init(renderer);
	}

	for (const auto& entity : m_entities) {
		entity->init(renderer);
	}
}

void Scene::update(float deltaTime)
{
	for (const auto& entity : m_entities) {
		entity->update(deltaTime);
	}
}

void Scene::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	auto renderTarget = renderer->getRenderTarget(m_renderTargetIndex);

	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->getOffscreenRenderPass());
	for (const auto& entity : m_entities) {
		entity->render(renderer, commandBuffer, currentFrame);
	}

	if (this->hasSkybox()) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}
	renderer->endRenderPass(commandBuffer);
}

void Scene::destroy(Renderer* renderer)
{
	if (this->hasSkybox()) {
		skybox->destroy(renderer);
	}

	for (const auto& entity : m_entities) {
		entity->destroy(renderer);
	}
}
