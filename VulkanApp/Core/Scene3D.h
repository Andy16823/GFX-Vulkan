#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Skybox.h"
#include "../Graphics/DirectionalLight.h"

class Scene3D 
	: public Scene
{
private:
	/// <summary>
	/// The entities in the scene
	/// </summary>
	std::vector<std::unique_ptr<Entity>> m_entities;

public:
	/// <summary>
	/// The skybox of the scene
	/// </summary>
	std::unique_ptr<Skybox> skybox;

	/// <summary>
	/// The directional light (Sun) of the scene
	/// </summary>
	std::unique_ptr<DirectionalLight> directionalLight;

	Scene3D();
	~Scene3D() = default;

	/// <summary>
	/// Checks if the scene has a skybox
	/// </summary>
	/// <returns></returns>
	bool hasSkybox() const {
		return skybox != nullptr;
	}

	/// <summary>
	/// Add an entity to the scene and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entity"></param>
	/// <returns></returns>
	template<typename T>
	T* addEntity(std::unique_ptr<Entity> entity) {
		T* casted = dynamic_cast<T*>(entity.get());
		if (casted == nullptr) {
			throw std::runtime_error("Failed to add entity: entity is not of the correct type!");
		}
		m_entities.push_back(std::move(entity));
		return casted;
	}

	/// <summary>
	/// Find all entities of type T in the scene
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	std::vector<T*> findEntities() const {
		std::vector<T*> foundEntities;
		for (const auto& entity : m_entities) {
			if (auto casted = dynamic_cast<T*>(entity.get())) {
				foundEntities.push_back(casted);
			}
		}
		return foundEntities;
	}

	/// <summary>
	/// Find an entity by name and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* findEntity(const std::string& name) const {
		for (const auto& entity : m_entities) {
			if (entity->name == name) {
				if (auto casted = dynamic_cast<T*>(entity.get())) {
					return casted;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Find an entity by type and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* findEntity() const {
		for (const auto& entity : m_entities) {
			if (auto casted = dynamic_cast<T*>(entity.get())) {
				return casted;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Find all entities by type and return them casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	std::vector<T*> findEntitiesByType() const {
		std::vector<T*> foundEntities;
		for (const auto& entity : m_entities) {
			if (auto casted = dynamic_cast<T*>(entity.get())) {
				foundEntities.push_back(casted);
			}
		}
		return foundEntities;
	}


	/// <summary>
	/// Find an entity by tag and return it casted to the correct type
	/// Returns the first entity found with the given tag
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="tagKey"></param>
	/// <returns></returns>
	template<typename T>
	T* findTaggedEntity(const std::string& tagKey) const {
		for (const auto& entity : m_entities) {
			if (entity->hasTag(tagKey)) {
				if (auto casted = dynamic_cast<T*>(entity.get())) {
					return casted;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Get all entities with a given tag
	/// </summary>
	/// <param name="tagKey"></param>
	/// <returns></returns>
	std::vector<Entity*> getTaggedEntities(const std::string& tagKey) const {
		std::vector<Entity*> taggedEntities;
		for (const auto& entity : m_entities) {
			if (entity->hasTag(tagKey)) {
				taggedEntities.push_back(entity.get());
			}
		}
		return taggedEntities;
	}

	/// <summary>
	/// Init the scene
	/// </summary>
	/// <param name="renderer"></param>
	void init(Renderer* renderer) override;

	/// <summary>
	/// Update the scene
	/// </summary>
	/// <param name="deltaTime"></param>
	void update(float deltaTime) override;

	/// <summary>
	/// Render the scene
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame) override;

	/// <summary>
	/// Destroy the scene
	/// </summary>
	/// <param name="renderer"></param>
	void destroy(Renderer* renderer) override;

	/// <summary>
	/// Bind the scene descriptor sets (e.g. lights) for the current pipeline
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	/// <param name="currentPipeline"></param>
	void bindSceneDescriptorSets(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame, const std::string& currentPipeline) override;


	RayHit raycast(const Ray& ray) const override;

};

