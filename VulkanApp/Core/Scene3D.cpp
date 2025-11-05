#include "Scene3D.h"

Scene3D::Scene3D()
{
	// Create the directional light and set its binding infos for the engine predefined pipelines
	this->directionalLight = std::make_unique<DirectionalLight>();
	this->directionalLight->addBindingInfo({ ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D), 6 });
	this->directionalLight->addBindingInfo({ ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED), 7 });
}

void Scene3D::init(Renderer* renderer)
{
	// Initialize the base scene
	Scene::init(renderer);

	// Initialize the skybox if it exists
	if (this->hasSkybox()) {
		skybox->init(renderer);
	}

	// Initialize all entities
	for (const auto& entity : m_entities) {
		entity->init(this, renderer);
	}

	// Initialize the directional light if it exists
	if (this->directionalLight != nullptr) {
		this->directionalLight->init(renderer);
	}
}

void Scene3D::update(float deltaTime)
{
	Scene::update(deltaTime);

	for (const auto& entity : m_entities) {
		if (entity->hasState(EntityState::ENTITY_STATE_ACTIVE)) {
			entity->update(this, deltaTime);
		}
	}
}

void Scene3D::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	// Get the render target for this scene
	int renderTargetIndex = this->getRenderTargetIndex();
	auto renderTarget = renderer->getRenderTarget(renderTargetIndex);

	// Beginn the render pass with the render target's framebuffer
	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->getOffscreenRenderPass());

	// TODO: Refactor this in the future to make it more straightforward
	Scene::render(renderer, commandBuffer, currentFrame);

	// Update the directional light buffers if it exists
	if (this->directionalLight != nullptr) {
		this->directionalLight->updateBuffers(renderer, commandBuffer, currentFrame);
	}

	// Render all visible entities
	for (const auto& entity : m_entities) {
		if (entity->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
			entity->render(this, renderer, commandBuffer, currentFrame);
		}
	}

	// Render the skybox if it exists
	if (this->hasSkybox()) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}

	// End the render pass
	renderer->endRenderPass(commandBuffer);
}

void Scene3D::destroy(Renderer* renderer)
{
	Scene::destroy(renderer);

	// Destroy the skybox if it exists
	if (this->hasSkybox()) {
		skybox->destroy(renderer);
	}

	// Destroy all entities
	for (const auto& entity : m_entities) {
		entity->destroy(this, renderer);
	}

	// Dispose the directional light if it exists
	if (this->directionalLight != nullptr) {
		this->directionalLight->dispose(renderer);
	}
}

void Scene3D::bindSceneDescriptorSets(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame, const std::string& currentPipeline)
{
	// Bind the directional light if it exists
	if (this->directionalLight != nullptr) {
		this->directionalLight->bind(renderer, commandBuffer, currentFrame, currentPipeline);
	}
}
