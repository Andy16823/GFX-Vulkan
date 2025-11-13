#pragma once
#include "../Core/Behavior.h"
#include "../Graphics/Camera.h"
#include "../Math/Frustum.h"
#include <glm/glm.hpp>

/// <summary>
/// The culling mode enumeration
/// </summary>
enum class CullingMode
{
	AABB_CULLING,				// Culling based on Axis-Aligned Bounding Box (expensive but accurate)
	SPHERE_CULLING,				// Culling based on Bounding Sphere (less expensive but less accurate)
	SPHERE_THEN_AABB_CULLING,	// First cull with bounding sphere, then with AABB (balanced approach)
	ORIGIN_CULLING				// Culling based on object origin point (cheap but very inaccurate)
};

/// <summary>
/// Class implementing frustum culling behavior
/// </summary>
class FrustumCullingBhv : public Behavior
{
private:
	/// <summary>
	/// Pointer to the camera used for culling
	/// </summary>
	Camera* m_camera;

	/// <summary>
	/// The culling mode to use
	/// </summary>
	CullingMode m_cullingMode = CullingMode::SPHERE_THEN_AABB_CULLING;

	/// <summary>
	/// Cached Axis-Aligned Bounding Box of the parent entity
	/// </summary>
	AABB m_aabb = AABB();

	/// <summary>
	/// The half extents of the bounding box
	/// </summary>
	glm::vec3 m_halfExtents = glm::vec3(0.0f);

	/// <summary>
	/// The last known position of the parent transform
	/// </summary>
	glm::vec3 m_lastParentPosition = glm::vec3(0.0f);

	/// <summary>
	/// The last known rotation of the parent transform
	/// </summary>
	glm::quat m_lastParentRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	/// <summary>
	/// The last known scale of the parent transform
	/// </summary>
	glm::vec3 m_lastParentScale = glm::vec3(1.0f);

	/// <summary>
	/// Checks if the transform has changed since the last check
	/// Updates the last known transform values if dirty
	/// </summary>
	/// <param name="transform"></param>
	/// <returns></returns>
	bool isDirty(Transform& transform);

public:

	/// <summary>
	/// Creates a frustum culling behavior with the specified camera and culling mode
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="mode"></param>
	FrustumCullingBhv(Camera* camera, CullingMode mode = CullingMode::SPHERE_THEN_AABB_CULLING);

	/// <summary>
	/// The initialization of the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void init(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// The update of the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="dt"></param>
	void update(Scene* scene, float dt) override;

	/// <summary>
	/// Destroys the behavior
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void destroy(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Renders the behavior (no-op for culling)
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="commandbuffer"></param>
	/// <param name="frame"></param>
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) override;

	/// <summary>
	/// Gets the unique identifier of this behavior
	/// </summary>
	/// <returns></returns>
	std::string getIdentifier() override;
};

