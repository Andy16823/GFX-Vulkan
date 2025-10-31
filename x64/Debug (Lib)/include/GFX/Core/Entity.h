#pragma once  
#define GLM_ENABLE_EXPERIMENTAL  
#include "../Graphics/Renderer.h"  
#include <glm/vec3.hpp>  
#include <glm/gtx/quaternion.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/mat4x4.hpp>  
#include "Behavior.h"
#include <vector>  
#include <type_traits>  
#include "../Math/Transform.h"

class Entity
{
private:
	std::vector<std::unique_ptr<Behavior>> m_behaviors;
	std::map<std::string, std::string> m_tags;

public:
	Transform transform;
	std::string name;
	std::string UniqueId;

	Entity(std::string name);
	virtual ~Entity() = default;

	// Transforms  
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setScale(glm::vec3 scale);
	glm::vec3 getScale();
	void setRotation(glm::quat rotation);
	glm::quat getRotation();
	void rotate(float x, float y, float z);
	UboModel getModelMatrix();

	// Virtual methods  
	virtual void update(float dt);
	virtual void init(Renderer* renderer) = 0;
	virtual void render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) = 0;
	virtual void destroy(Renderer* renderer) = 0;
	
	void addTag(const std::string& key, const std::string& value)
	{
		m_tags[key] = value;
	}

	bool hasTag(const std::string& key)
	{
		return m_tags.find(key) != m_tags.end();
	}

	template<typename T>
	T* addBehavior(std::unique_ptr<T> behavior)
	{
		static_assert(std::is_base_of<Behavior, T>::value, "T must be a subclass of Component");
		behavior->parent = this;
		T* raw = behavior.get();
		m_behaviors.push_back(std::move(behavior));
		return raw;
	}

	// Template functions
	template<typename T>
	T* findComponent()
	{
		for (auto& component : m_behaviors) {
			if (auto casted = dynamic_cast<T*>(component.get())) {
				return casted;
			}
		}
		return nullptr;
	}
};
