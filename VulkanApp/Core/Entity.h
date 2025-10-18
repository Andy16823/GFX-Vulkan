#pragma once  
#define GLM_ENABLE_EXPERIMENTAL  
#include "../Graphics/Renderer.h"  
#include <glm/vec3.hpp>  
#include <glm/gtx/quaternion.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/mat4x4.hpp>  
#include "Component.h"
#include <vector>  
#include <type_traits>  

class Entity
{
private:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	std::vector<std::unique_ptr<Component>> m_components;
public:
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

	// Own components  
	void addComponent(Component* component);
	
	template<typename T>
	T* addComponent(std::unique_ptr<T> component)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a subclass of Component");
		component->parent = this;
		T* raw = component.get();
		m_components.push_back(std::move(component));
		return raw;
	}

	// Template functions
	template<typename T>
	T* findComponent()
	{
		for (auto& component : m_components) {
			if (auto casted = dynamic_cast<T*>(component.get())) {
				return casted;
			}
		}
		return nullptr;
	}
};
