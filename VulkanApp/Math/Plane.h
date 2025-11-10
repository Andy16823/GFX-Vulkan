#pragma once
#include <glm/glm.hpp>

struct Plane
{
	glm::vec3 normal;
	float d;

	float distance(const glm::vec3& point) const {
		return glm::dot(normal, point) + d;
	}

	// Erzeuge eine Ebene aus 3 Punkten. Nutze eine konsistente Kreuzprodukt-Reihenfolge.
	static Plane fromPoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
		// Standard: cross(p2 - p1, p3 - p1)
		glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
		float d = -glm::dot(n, p1);
		return Plane{ n, d };
	}
};