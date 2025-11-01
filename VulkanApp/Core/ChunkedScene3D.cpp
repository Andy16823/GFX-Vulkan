#include "ChunkedScene3D.h"

void ChunkedScene3D::init(Renderer* renderer)
{
	// Call base init to create render target
	Scene::init(renderer);

	// Init all chunk entities
	for (const auto& [chunkIndex, entities] : m_chunks)
	{
		for (const auto& entity : entities)
		{
			entity->init(renderer);
		}
	}

	// Init all global entities
	for (const auto& entity : m_globalEntities)
	{
		entity->init(renderer);
	}

	// Init the skybox if it exists
	if (this->skybox != nullptr)
	{
		skybox->init(renderer);
	}
}

void ChunkedScene3D::update(float deltaTime)
{
	// Update the current chunk
	auto it = m_chunks.find(m_currentChunk);
	if (it != m_chunks.end()) {
		const auto& currentChunkEntities = it->second;
		for (const auto& entity : currentChunkEntities) {
			if (entity->hasState(EntityState::ENTITY_STATE_ACTIVE)) {
				entity->update(deltaTime);
			}
		}
	}

	// Update the neighboring chunks
	auto neighbors = this->getChunkNeighbors(m_currentChunk);
	for (const auto& neighborIndex : neighbors)
	{
		auto neighborsIt = m_chunks.find(neighborIndex);
		if (neighborsIt != m_chunks.end()) {
			const auto& neighborEntities = neighborsIt->second;
			for (const auto& entity : neighborEntities) {
				if (entity->hasState(EntityState::ENTITY_STATE_ACTIVE)) {
					entity->update(deltaTime);
				}
			}
		}
	}

	// Update global entities
	for (const auto& entity : m_globalEntities)
	{
		if (entity->hasState(EntityState::ENTITY_STATE_ACTIVE))
		{
			entity->update(deltaTime);
		}
	}
}

void ChunkedScene3D::render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	int renderTargetIndex = this->getRenderTargetIndex();
	auto renderTarget = renderer->getRenderTarget(renderTargetIndex);

	renderer->beginnRenderPass(commandBuffer, renderTarget->getFramebuffer(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->getOffscreenRenderPass());

	// Render current chunk entities
	auto it = m_chunks.find(m_currentChunk);
	if (it != m_chunks.end()) {
		const auto& currentChunkEntities = it->second;
		for (const auto& entity : currentChunkEntities) {
			if (entity->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
				entity->render(renderer, commandBuffer, currentFrame);
			}
		}
	}

	// Render neighboring chunk entities
	auto neighbors = this->getChunkNeighbors(m_currentChunk);
	for (const auto& neighborIndex : neighbors) {
		auto neighborsIt = m_chunks.find(neighborIndex);
		if (neighborsIt != m_chunks.end()) {
			const auto& neighborEntities = neighborsIt->second;
			for (const auto& entity : neighborEntities) {
				if (entity->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
					entity->render(renderer, commandBuffer, currentFrame);
				}
			}
		}
	}

	// Render global entities
	for (const auto& entity : m_globalEntities) {
		if (entity->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
			entity->render(renderer, commandBuffer, currentFrame);
		}
	}

	// Render the skybox if it exists
	if (this->skybox != nullptr) {
		skybox->render(renderer, commandBuffer, currentFrame);
	}

	renderer->endRenderPass(commandBuffer);
}

void ChunkedScene3D::destroy(Renderer* renderer)
{
	// Destroy all chunk entities 
	for (const auto& [chunkIndex, entities] : m_chunks) {
		for (const auto& chunkEntity : entities) {
			chunkEntity->destroy(renderer);
		}
	}

	// Destroy all global entities
	for (const auto& entity : m_globalEntities) {
		entity->destroy(renderer);
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
	m_currentChunk = this->getChunkForPosition(position);
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

