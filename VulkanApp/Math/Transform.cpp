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

glm::vec3 Transform::worldToNDC(const glm::vec3& localCoords, glm::mat4 projection, glm::mat4 view)
{
	glm::mat4 mv = projection * view;
	glm::vec4 worldCoords = mv * glm::vec4(localCoords, 1.0f);
	return glm::vec3(worldCoords) / worldCoords.w;
}

glm::vec3 Transform::ndcToWorld(const glm::vec3& ndcCoords, glm::mat4 projection, glm::mat4 view)
{
	glm::mat4 invMV = glm::inverse(projection * view);
	glm::vec4 clipCoords = glm::vec4(ndcCoords, 1.0f);
	glm::vec4 worldCoords = invMV * clipCoords;
	if (worldCoords.w != 0.0f) {
		return glm::vec3(worldCoords) / worldCoords.w;
	}
	return glm::vec3(0.0f);
}
