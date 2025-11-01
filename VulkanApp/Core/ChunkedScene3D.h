#pragma once
#include "Scene.h"
#include "Entity.h"
#include <map>
#include <vector>

/// <summary>
/// The chunk index structure
/// </summary>
struct ChunkIndex {
	int chunkX;
	int chunkY;
	int chunkZ;

	auto operator<=>(const ChunkIndex&) const = default;
	bool operator==(const ChunkIndex&) const = default;
};

/// <summary>
/// Scene for streaming large 3D worlds in chunks
/// </summary>
class ChunkedScene3D : 
	public Scene
{
private:
	/// <summary>
	/// The chunks in the scene
	/// </summary>
	std::map<ChunkIndex, std::vector<std::unique_ptr<Entity>>> m_chunks;

	/// <summary>
	/// Global entities that are not part of any chunk
	/// </summary>
	std::vector<std::unique_ptr<Entity>> m_globalEntities;				

	/// <summary>
	/// The current active chunk
	/// </summary>
	ChunkIndex m_currentChunk = { 0, 0, 0 };
public:
	/// <summary>
	/// The skybox of the scene
	/// </summary>
	std::unique_ptr<Skybox> skybox;

	float chunkSize = 100.0f;

	void init(Renderer* renderer) override;
	void update(float deltaTime) override;
	void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
	void destroy(Renderer* renderer) override;

	/// <summary>
	/// Get the chunk data for a given world position
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	ChunkIndex getChunkForPosition(const glm::vec3& position);

	/// <summary>
	/// Add an entity to the appropriate chunk based on its position
	/// </summary>
	/// <param name="entity"></param>
	void addEntityToChunk(std::unique_ptr<Entity> entity);

	/// <summary>
	/// Adds an entity that is not part of any chunk and is always active
	/// This is useful for entities like the player or UI elements
	/// </summary>
	/// <param name="entity"></param>
	template<typename T>
	T* addGlobalEntity(std::unique_ptr<Entity> entity) {
		T* casted = dynamic_cast<T*>(entity.get());
		if (!casted) {
			throw std::runtime_error("Failed to add global entity: invalid type cast");
		}
		m_globalEntities.push_back(std::move(entity));
		return casted;
	}

	/// <summary>
	/// Find a global entity by name and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* findGlobalEntity(const std::string& name) const {
		for (const auto& entity : m_globalEntities) {
			if (entity->name == name) {
				if (auto casted = dynamic_cast<T*>(entity.get())) {
					return casted;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Find a chunk entity by name and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="chunkIndex"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* findChunkEntity(const ChunkIndex& chunkIndex, const std::string& name) const {
		auto it = m_chunks.find(chunkIndex);
		if (it == m_chunks.end()) {
			return nullptr;
		}
		const auto& chunkdata = it->second;
		for (const auto& entity : chunkdata) {
			if (entity->name == name) {
				if (auto casted = dynamic_cast<T*>(entity.get())) {
					return casted;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Set the active chunk based on a world position
	/// </summary>
	/// <param name="position"></param>
	void setActiveChunk(const glm::vec3& position);

	/// <summary>
	///	Returns the neighboring chunk indices for a given chunk index
	/// </summary>
	/// <param name="chunkIndex"></param>
	/// <returns></returns>
	std::vector<ChunkIndex> getChunkNeighbors(const ChunkIndex& chunkIndex);
};

