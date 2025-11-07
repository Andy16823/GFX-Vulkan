#include "ChunkedScene3D.h"

ChunkedScene3D::ChunkedScene3D(glm::vec3 initialPosition) : Scene()
{
	// Set the starting chunk and its neighbors
	m_currentChunk = this->getChunkForPosition(initialPosition);
	m_neighboringChunks = this->getChunkNeighbors(m_currentChunk);

	// Create the directional light and set its binding infos for the engine predefined pipelines
	this->directionalLight = std::make_unique<DirectionalLight>();
	this->directionalLight->addBindingInfo({ ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D), 6 });
	this->directionalLight->addBindingInfo({ ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED), 7 });
}

void ChunkedScene3D::init(Renderer* renderer)
{
	// Call base init to create render target
	Scene::init(renderer);

	// Init all chunk entities
	for (const auto& [chunkIndex, entities] : m_chunks)
	{
		for (const auto& entity : entities)
		{
			entity->init(this, renderer);
		}
	}

	// Init all global entities
	for (const auto& entity : m_globalEntities)
	{
		entity->init(this, renderer);
	}

	// Init the skybox if it exists
	if (this->skybox != nullptr)
	{
		skybox->init(renderer);
	}

	// Init the directional light if it exists
	if (this->directionalLight != nullptr) {
		this->directionalLight->init(renderer);
	}
}

void ChunkedScene3D::update(float deltaTime)
{
	Scene::update(deltaTime);

	// Update the current chunk
	auto it = m_chunks.find(m_currentChunk);
	if (it != m_chunks.end()) {
		const auto& currentChunkEntities = it->second;
		for (const auto& entity : currentChunkEntities) {
			entity->update(this, deltaTime);
		}
	}

	// Update the neighboring chunks
	for (const auto& neighborIndex : m_neighboringChunks)
	{
		auto neighborsIt = m_chunks.find(neighborIndex);
		if (neighborsIt != m_chunks.end()) {
			const auto& neighborEntities = neighborsIt->second;
			for (const auto& entity : neighborEntities) {
				entity->update(this, deltaTime);
			}
		}
	}

	// Update global entities
	for (const auto& entity : m_globalEntities)
	{
		entity->update(this, deltaTime);
	}
}

void ChunkedScene3D::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	int renderTargetIndex = this->getRenderTargetIndex();
	auto renderTarget = renderer->getRenderTarget(renderTargetIndex);

	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->getOffscreenRenderPass());

	Scene::render(renderer, commandBuffer, currentFrame);

	// Update the light buffers and bind the light
	if (this->directionalLight != nullptr) {
		this->directionalLight->updateBuffers(renderer, commandBuffer, currentFrame);
	}

	// Render current chunk entities
	auto it = m_chunks.find(m_currentChunk);
	if (it != m_chunks.end()) {
		const auto& currentChunkEntities = it->second;
		for (const auto& entity : currentChunkEntities) {
			entity->render(this, renderer, commandBuffer, currentFrame);
		}
	}

	// Render neighboring chunk entities
	for (const auto& neighborIndex : m_neighboringChunks) {
		auto neighborsIt = m_chunks.find(neighborIndex);
		if (neighborsIt != m_chunks.end()) {
			const auto& neighborEntities = neighborsIt->second;
			for (const auto& entity : neighborEntities) {
				entity->render(this, renderer, commandBuffer, currentFrame);
			}
		}
	}

	// Render global entities
	for (const auto& entity : m_globalEntities) {
		entity->render(this, renderer, commandBuffer, currentFrame);
	}

	// Render the skybox if it exists
	if (this->skybox != nullptr) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}

	renderer->endRenderPass(commandBuffer);
}

void ChunkedScene3D::destroy(Renderer* renderer)
{
	Scene::destroy(renderer);

	// Destroy all chunk entities 
	for (const auto& [chunkIndex, entities] : m_chunks) {
		for (const auto& chunkEntity : entities) {
			chunkEntity->destroy(this, renderer);
		}
	}

	// Destroy all global entities
	for (const auto& entity : m_globalEntities) {
		entity->destroy(this, renderer);
	}

	// Destroy the skybox if it exists
	if (this->skybox != nullptr) {
		skybox->destroy(renderer);
	}
}

ChunkIndex ChunkedScene3D::getChunkForPosition(const glm::vec3& position)
{
	ChunkIndex index;
	index.chunkX = static_cast<int>(floor(position.x / chunkSize));
	index.chunkY = static_cast<int>(floor(position.y / chunkSize));
	index.chunkZ = static_cast<int>(floor(position.z / chunkSize));
	return index;
}

void ChunkedScene3D::addEntityToChunk(std::unique_ptr<Entity> entity)
{
	ChunkIndex index = this->getChunkForPosition(entity->getPosition());
	m_chunks[index].push_back(std::move(entity));
}

void ChunkedScene3D::setActiveChunk(const glm::vec3& position)
{
	auto chunkIndex = this->getChunkForPosition(position);
	if (chunkIndex == m_currentChunk) {
		return;
	}
	m_currentChunk = chunkIndex;
	m_neighboringChunks = this->getChunkNeighbors(m_currentChunk);
}

std::vector<ChunkIndex> ChunkedScene3D::getChunkNeighbors(const ChunkIndex& chunkIndex)
{
	std::vector<ChunkIndex> neighbors;
	neighbors.reserve(26);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			for (int z = -1; z <= 1; ++z) {
				if (x == 0 && y == 0 && z == 0) {
					continue;
				}
				neighbors.push_back({
					chunkIndex.chunkX + x,
					chunkIndex.chunkY + y,
					chunkIndex.chunkZ + z
				});
			}
		}
	}
	return neighbors;
}

void ChunkedScene3D::bindSceneDescriptorSets(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame, const std::string& currentPipeline)
{
	if (this->directionalLight != nullptr) {
		this->directionalLight->bind(renderer, commandBuffer, currentFrame, currentPipeline);
	}
}

RayHit ChunkedScene3D::raycast(const Ray& ray) const
{
	float closestDistance = std::numeric_limits<float>::max();
	Entity* hitEntity = nullptr;

	// Check global entities
	for (const auto& entity : m_globalEntities) {
		AABB worldAabb = entity->getAABB(true);
		float tMin, tMax;
		if (RayCast::rayIntersectsAABB(ray, worldAabb, tMin, tMax)) {
			if (tMin < closestDistance) {
				closestDistance = tMin;
				hitEntity = entity.get();
			}
		}
	}

	// Check current chunk entities
	auto it = m_chunks.find(m_currentChunk);
	if (it != m_chunks.end()) {
		const auto& entities = it->second;
		for (const auto& entity : entities) {
			AABB worldAabb = entity->getAABB(true);
			float tMin, tMax;
			if (RayCast::rayIntersectsAABB(ray, worldAabb, tMin, tMax)) {
				if (tMin < closestDistance) {
					closestDistance = tMin;
					hitEntity = entity.get();
				}
			}
		}
	}

	// check neighboring chunk entities 
	for (const auto& neighborIndex : m_neighboringChunks) {
		auto neighborsIt = m_chunks.find(neighborIndex);
		if (neighborsIt != m_chunks.end()) {
			const auto& entities = neighborsIt->second;
			for (const auto& entity : entities) {
				AABB worldAabb = entity->getAABB(true);
				float tMin, tMax;
				if (RayCast::rayIntersectsAABB(ray, worldAabb, tMin, tMax)) {
					if (tMin < closestDistance) {
						closestDistance = tMin;
						hitEntity = entity.get();
					}
				}
			}
		}
	}

	RayHit hitResult = {};
	hitResult.hitobject = hitEntity;
	hitResult.distance = (hitEntity != nullptr) ? closestDistance : -1.0f;
	hitResult.position = ray.origin + ray.direction * closestDistance;
	hitResult.hit = (hitEntity != nullptr);
	return hitResult;
}

