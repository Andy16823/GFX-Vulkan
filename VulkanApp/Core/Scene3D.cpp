#include "Scene3D.h"

void Scene3D::init(Renderer* renderer)
{
	Scene::init(renderer);	// Call base init to create render target
	if (this->hasSkybox()) {
		skybox->init(renderer);
	}

	for (const auto& entity : m_entities) {
		entity->init(renderer);
	}
}

void Scene3D::update(float deltaTime)
{
	for (const auto& entity : m_entities) {
		if (entity->hasState(EntityState::ENTITY_STATE_ACTIVE)) {
			entity->update(deltaTime);
		}
	}
}

void Scene3D::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	int renderTargetIndex = this->getRenderTargetIndex();
	auto renderTarget = renderer->getRenderTarget(renderTargetIndex);

	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->getOffscreenRenderPass());
	for (const auto& entity : m_entities) {
		if (entity->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
			entity->render(renderer, commandBuffer, currentFrame);
		}
	}

	if (this->hasSkybox()) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}
	renderer->endRenderPass(commandBuffer);
}

void Scene3D::destroy(Renderer* renderer)
{
	if (this->hasSkybox()) {
		skybox->destroy(renderer);
	}

	for (const auto& entity : m_entities) {
		entity->destroy(renderer);
	}
}
