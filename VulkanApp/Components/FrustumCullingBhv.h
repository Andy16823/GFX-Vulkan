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
	Camera* m_camera;
	CullingMode m_cullingMode = CullingMode::SPHERE_THEN_AABB_CULLING;
	AABB m_aabb = AABB();

	glm::vec3 m_halfExtents = glm::vec3(0.0f);

	glm::vec3 m_lastParentPosition = glm::vec3(0.0f);
	glm::quat m_lastParentRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_lastParentScale = glm::vec3(1.0f);

	/// <summary>
	/// Checks if the transform has changed since the last check
	/// Updates the last known transform values if dirty
	/// </summary>
	/// <param name="transform"></param>
	/// <returns></returns>
	bool isDirty(Transform& transform);

public:
	FrustumCullingBhv(Camera* camera, CullingMode mode = CullingMode::SPHERE_THEN_AABB_CULLING);

	void init(Scene* scene, Renderer* renderer) override;

	void update(Scene* scene, float dt) override;

	void destroy(Scene* scene, Renderer* renderer) override;

	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) override;

	std::string getIdentifier() override;
};

