#include "RayCast.h"

Ray RayCast::createRayFromCamera(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, bool invertZ /*= true*/)
{
	glm::vec3 nearPoint = camera->unprojectPosition(glm::vec3(mousePos, 0.0f), viewport);
	glm::vec3 farPoint = camera->unprojectPosition(glm::vec3(mousePos, 1.0f), viewport);
	glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);
	if (invertZ) {
		nearPoint.z = -nearPoint.z;
		rayDir.z = -rayDir.z;
	}
	return Ray{ nearPoint, rayDir };
}

glm::vec3 RayCast::castRayFromMouse(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, float groundHeight, bool invertZ)
{
	glm::vec3 nearPoint = camera->unprojectPosition(glm::vec3(mousePos, 0.0f), viewport);
	glm::vec3 farPoint = camera->unprojectPosition(glm::vec3(mousePos, 1.0f), viewport);
	glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

	if (std::abs(rayDir.y) < 0.0001f) {
		return glm::vec3(0.0f);
	}

	float t = (groundHeight - nearPoint.y) / rayDir.y;

	if (t > 0.0f) {
		glm::vec3 result = nearPoint + rayDir * t;
		if (invertZ) {
			result.z = -result.z;
		}
		return result;
	}

	return glm::vec3(0.0f);
}

bool RayCast::rayIntersectsAABB(const Ray& ray, const struct AABB& aabb, float& tMin, float& tMax)
{
	tMin = 0.0f;
	tMax = FLT_MAX;

	for (int i = 0; i < 3; i++) {
		float invD = 1.0f / ray.direction[i];
		float t0 = (aabb.min[i] - ray.origin[i]) * invD;
		float t1 = (aabb.max[i] - ray.origin[i]) * invD;

		if (invD < 0.0f) {
			std::swap(t0, t1);
		}

		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		if (tMax <= tMin) {
			return false;
		}
	}
	return true;
}
