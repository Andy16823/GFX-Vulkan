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
#include "../Math/AABB.h"

class Scene;

/// <summary>
/// 
/// </summary>
enum class EntityState : uint8_t {
	ENTITY_STATE_NONE = 0,
	ENTITY_STATE_ACTIVE = 1 << 0,
	ENTITY_STATE_VISIBLE = 1 << 1
};

/// <summary>
/// Enum bitwise OR operator overload
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline EntityState operator|(EntityState a, EntityState b)
{
	return static_cast<EntityState>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

/// <summary>
/// Enum bitwise AND operator overload
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline EntityState operator&(EntityState a, EntityState b)
{
	return static_cast<EntityState>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

/// <summary>
/// Bitwise NOT operator overload
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
inline EntityState operator~(EntityState a)
{
	return static_cast<EntityState>(~static_cast<uint8_t>(a));
}

/// <summary>
/// Bitwise OR assignment operator overload
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline EntityState operator|=(EntityState a, EntityState b) {
	return a = a | b;
}

/// <summary>
/// Bitwise AND assignment operator overload
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline EntityState operator&=(EntityState a, EntityState b) {
	return a = a & b;
}

/// <summary>
/// Helpers to check if a flag is set in an EntityState
/// </summary>
/// <param name="state"></param>
/// <param name="flag"></param>
/// <returns></returns>
inline bool hasFlag(EntityState state, EntityState flag) {
	return (state & flag) == flag;
}

/// <summary>
/// Header for the Entity class
/// </summary>
class Entity
{
private:
	/// <summary>
	/// The behaviors attached to this entity
	/// </summary>
	std::vector<std::unique_ptr<Behavior>> m_behaviors;

	/// <summary>
	/// The tags attached to this entity
	/// </summary>
	std::map<std::string, std::string> m_tags;

	/// <summary>
	/// The axis-aligned bounding box of the entity
	/// </summary>
	AABB m_aabb;

	/// <summary>
	/// The state of the entity
	/// </summary>
	EntityState m_state = EntityState::ENTITY_STATE_ACTIVE | EntityState::ENTITY_STATE_VISIBLE;
public:
	/// <summary>
	/// The transform of the entity
	/// </summary>
	Transform transform;

	/// <summary>
	/// The name of the entity
	/// </summary>
	std::string name;

	/// <summary>
	/// The unique ID of the entity
	/// </summary>
	std::string uuId;

	/// <summary>
	/// The pipeline type used to render the entity
	/// </summary>
	std::string pipelineType;

	Entity(std::string name);
	virtual ~Entity() = default;

	/// <summary>
	/// Sets the position of the entity
	/// </summary>
	/// <param name="position"></param>
	void setPosition(glm::vec3 position);

	/// <summary>
	/// Gets the position of the entity
	/// </summary>
	/// <returns></returns>
	glm::vec3 getPosition();

	/// <summary>
	/// Sets the scale of the entity
	/// </summary>
	/// <param name="scale"></param>
	void setScale(glm::vec3 scale);

	/// <summary>
	/// Gets the scale of the entity
	/// </summary>
	/// <returns></returns>
	glm::vec3 getScale();

	/// <summary>
	/// Sets the rotation of the entity
	/// </summary>
	/// <param name="rotation"></param>
	void setRotation(glm::quat rotation);

	/// <summary>
	/// Gets the rotation of the entity
	/// </summary>
	/// <returns></returns>
	glm::quat getRotation();

	/// <summary>
	/// Rotates the entity by the given euler angles
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void rotate(float x, float y, float z);

	/// <summary>
	/// Gets the model matrix of the entity for shader use
	/// </summary>
	/// <returns></returns>
	UboModel getModelMatrix();

	/// <summary>
	/// Update the entity
	/// </summary>
	/// <param name="dt"></param>
	virtual void update(Scene* scene, float dt);

	/// <summary>
	/// Initialize the entity
	/// </summary>
	/// <param name="renderer"></param>
	virtual void init(Scene* scene, Renderer* renderer) = 0;

	/// <summary>
	/// Render the entity
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	virtual void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) = 0;

	/// <summary>
	/// Destroy the entity
	/// </summary>
	/// <param name="renderer"></param>
	virtual void destroy(Scene* scene, Renderer* renderer) = 0;
	
	/// <summary>
	/// Creates the axis-aligned bounding box for the entity
	/// Gets called during initialization
	/// </summary>
	virtual void createAABB() = 0;

	/// <summary>
	/// Gets the axis-aligned bounding box of the entity
	/// </summary>
	/// <param name="worldpos">Defines whether to get the AABB in world position or local position</param>
	/// <returns></returns>
	virtual AABB getAABB(bool worldpos) {
		return m_aabb * (worldpos ? transform.getMatrix() : glm::mat4(1.0f));
	}

	/// <summary>
	/// Sets the axis-aligned bounding box of the entity
	/// </summary>
	/// <param name="aabb"></param>
	void setAABB(const AABB& aabb) {
		m_aabb = aabb;
	}

	/// <summary>
	/// Add a tag to the entity
	/// </summary>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void addTag(const std::string& key, const std::string& value)
	{
		m_tags[key] = value;
	}

	/// <summary>
	/// Checks if the entity has a tag
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool hasTag(const std::string& key)
	{
		return m_tags.find(key) != m_tags.end();
	}

	/// <summary>
	/// Add a behavior to the entity and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="behavior"></param>
	/// <returns></returns>
	template<typename T>
	T* addBehavior(std::unique_ptr<T> behavior)
	{
		static_assert(std::is_base_of<Behavior, T>::value, "T must be a subclass of Component");
		behavior->parent = this;
		T* raw = behavior.get();
		m_behaviors.push_back(std::move(behavior));
		return raw;
	}

	/// <summary>
	/// Find a behavior by type and return it casted to the correct type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* findBehavior() const
	{
		for (auto& component : m_behaviors) {
			if (auto casted = dynamic_cast<T*>(component.get())) {
				return casted;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Gets the state of the entity
	/// </summary>
	/// <returns></returns>
	EntityState getState() const {
		return m_state;
	}

	/// <summary>
	/// Sets the state of the entity
	/// </summary>
	/// <param name="state"></param>
	void setState(EntityState state) {
		m_state = state;
	}

	/// <summary>
	/// Checks if the entity has the given state
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	bool hasState(EntityState state) const {
		return hasFlag(m_state, state);
	}
};
