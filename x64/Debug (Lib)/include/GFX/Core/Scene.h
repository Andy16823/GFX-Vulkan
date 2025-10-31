#pragma once
#include "../Graphics/Renderer.h"
#include <vector>
#include "Entity.h"
#include "Skybox.h"

/// <summary>
/// Scene class
/// </summary>
class Scene
{
private:
	/// <summary>
	/// The entities in the scene
	/// </summary>
	std::vector<std::unique_ptr<Entity>> m_entities;

	/// <summary>
	/// The render target index for this scene
	/// </summary>
	int m_renderTargetIndex = -1;

public:

	/// <summary>
	/// The skybox of the scene
	/// </summary>
	std::unique_ptr<Skybox> skybox;

	/// <summary>
	/// init the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Renderer* renderer);

	/// <summary>
	/// update the scene
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime);

	/// <summary>
	/// render the scene
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame);

	/// <summary>
	/// destroy the scene
	/// </summary>
	/// <param name="renderer"></param>
	virtual void destroy(Renderer* renderer);

	Scene() = default;
	~Scene() = default;

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
	std::vector<T*> findEntities() {
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
	T* findEntity(std::string name) {
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
	/// Find an entity by tag and return it casted to the correct type
	/// Returns the first entity found with the given tag
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="tagKey"></param>
    /// <returns></returns>
    template<typename T>
    T* findTaggedEntity(std::string tagKey) {
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
	std::vector<Entity*> getTaggedEntities(std::string tagKey) {
		std::vector<Entity*> taggedEntities;
		for (const auto& entity : m_entities) {
			if (entity->hasTag(tagKey)) {
				taggedEntities.push_back(entity.get());
			}
		}
		return taggedEntities;
	}
};

