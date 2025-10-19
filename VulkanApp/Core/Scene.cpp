#include "Scene.h"

void Scene::init(Renderer* renderer)
{
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
	for (const auto& entity : m_entities) {
		entity->render(renderer, commandBuffer, currentFrame);
	}

	if (this->hasSkybox()) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}
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

void Scene::addEntity(std::unique_ptr<Entity> entity)
{
	m_entities.push_back(std::move(entity));
}
