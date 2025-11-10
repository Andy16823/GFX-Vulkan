#pragma once
#include "Entity.h"

// Forward declaration
class Instancer;

/// <summary>
/// Instance Handle Entity
/// </summary>
class InstanceHandle : public Entity
{
private:
	/// <summary>
	/// The related instancer for this handle
	/// </summary>
	Instancer* m_instancer;

	EntityState m_previousState = EntityState::ENTITY_STATE_NONE;

	bool isDirty();
public:
	/// <summary>
	/// The ID of this instance within the instancer
	/// </summary>
	int instanceID;

	/// <summary>
	/// Create an instance handle for a specific instancer and instance ID
	/// Use Instancer::createInstanceHandle to create instance handles
	/// Never create directly to ensure proper management
	/// </summary>
	/// <param name="name"></param>
	/// <param name="model"></param>
	/// <param name="instanceId"></param>
	InstanceHandle(const std::string& name, Instancer* model, int instanceId);

	/// <summary>
	/// Initialize the instance handle
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void init(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Render the instance handle (update its instance data)
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;

	/// <summary>
	/// Destroy the instance handle
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void destroy(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Update the instance handle
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="dt"></param>
	void update(Scene* scene, float dt) override;

	/// <summary>
	/// Create the axis-aligned bounding box for this instance handle
	/// </summary>
	void createAABB() override;
};

