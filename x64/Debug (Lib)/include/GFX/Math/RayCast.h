#pragma once
#include <glm/glm.hpp>
#include "../Graphics/Camera.h"
#include "AABB.h"

/// <summary>
/// Information about a ray
/// </summary>
struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

/// <summary>
/// Ray hit information
/// </summary>
struct RayHit
{
	bool hit;
	glm::vec3 position;
	float distance;
	void* hitobject;
};

/// <summary>
/// Static class for ray casting operations
/// </summary>
class RayCast
{
public:
	/// <summary>
	/// Creates a ray from the camera through the mouse position
	/// </summary>
	/// <param name="mousePos"></param>
	/// <param name="viewport"></param>
	/// <param name="camera"></param>
	/// <param name="invertZ"></param>
	/// <returns></returns>
	static Ray createRayFromCamera(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, bool invertZ = true);

	/// <summary>
	/// Casts a ray from the mouse position onto a ground plane at a specific height
	/// </summary>
	/// <param name="mousePos"></param>
	/// <param name="viewport"></param>
	/// <param name="camera"></param>
	/// <param name="groundHeight"></param>
	/// <param name="invertZ"></param>
	/// <returns></returns>
	static glm::vec3 castRayFromMouse(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, float groundHeight = 0.0f, bool invertZ = true);

	/// <summary>
	/// Checks if a ray intersects an axis-aligned bounding box (AABB)
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="aabb"></param>
	/// <param name="tMin"></param>
	/// <param name="tMax"></param>
	/// <returns></returns>
	static bool rayIntersectsAABB(const Ray& ray, const struct AABB& aabb, float& tMin, float& tMax);
};

