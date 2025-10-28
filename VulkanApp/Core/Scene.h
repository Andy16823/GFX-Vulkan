#pragma once
#include "../Graphics/Renderer.h"
#include <vector>
#include "Entity.h"
#include "Skybox.h"

class Scene
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	int m_renderTargetIndex = -1;

public:
	std::unique_ptr<Skybox> skybox;

	virtual void init(Renderer* renderer);
	virtual void update(float deltaTime);
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t currentFrame);
	virtual void destroy(Renderer* renderer);

	Scene() = default;
	~Scene() = default;

	void addEntity(std::unique_ptr<Entity> entity);

	bool hasSkybox() const {
		return skybox != nullptr;
	}

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

};

