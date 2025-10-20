#include "Transform.h"

void Transform::towards(glm::vec3 target)
{
	glm::vec3 direction = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Check for gimbal lock
	if (glm::abs(glm::dot(direction, up)) > 0.9999f) {
		up = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	rotation = glm::quatLookAt(direction, up);
}
