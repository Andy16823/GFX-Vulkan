#pragma once
#include <glm/glm.hpp>
#include "../Graphics/Camera.h"
#include "AABB.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

struct RayHit
{
	bool hit;
	glm::vec3 position;
	float distance;
	void* hitobject;
};

class RayCast
{
public:
	static Ray createRayFromCamera(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, bool invertZ = true);
	static glm::vec3 castRayFromMouse(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, float groundHeight = 0.0f, bool invertZ = true);
	static bool rayIntersectsAABB(const Ray& ray, const struct AABB& aabb, float& tMin, float& tMax);
};

