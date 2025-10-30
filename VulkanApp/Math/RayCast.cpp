#include "RayCast.h"

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
