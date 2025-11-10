#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "Plane.h"
#include "AABB.h"
#include <vector>

struct Frustum
{
	glm::vec3 nearTopLeft;
	glm::vec3 nearTopRight;
	glm::vec3 nearBottomLeft;
	glm::vec3 nearBottomRight;
	glm::vec3 farTopLeft;
	glm::vec3 farTopRight;
	glm::vec3 farBottomLeft;
	glm::vec3 farBottomRight;

	std::vector<Plane> getPlanes() const {
		std::vector<Plane> planes;
		// Erzeuge Ebenen mit konsistenter Winding-Order (Cross-Reihenfolge p2-p1, p3-p1)
		planes.push_back(Plane::fromPoints(nearTopLeft, nearTopRight, nearBottomRight));      // Near
		planes.push_back(Plane::fromPoints(farTopRight, farTopLeft, farBottomLeft));          // Far
		planes.push_back(Plane::fromPoints(nearTopLeft, farTopLeft, farBottomLeft));          // Left
		planes.push_back(Plane::fromPoints(nearTopRight, farTopRight, farBottomRight));       // Right
		planes.push_back(Plane::fromPoints(nearTopLeft, farTopLeft, farTopRight));            // Top
		planes.push_back(Plane::fromPoints(nearBottomLeft, farBottomLeft, farBottomRight));   // Bottom

		// Inneren Bezugspunkt (Mittelpunkt der 8 Ecken)
		glm::vec3 center = (nearTopLeft + nearTopRight + nearBottomLeft + nearBottomRight +
			farTopLeft + farTopRight + farBottomLeft + farBottomRight) * (1.0f / 8.0f);

		// Drehe jede Ebene, falls center auf negativer Seite liegt -> dann ist center dist < 0
		for (auto& pl : planes) {
			if (pl.distance(center) < 0.0f) {
				pl.normal = -pl.normal;
				pl.d = -pl.d;
			}
		}

		return planes;
	}

	bool intersectsAABB(const AABB& aabb) const {
		auto planes = getPlanes();

		for (const auto& plane : planes) {
			// wähle Eckpunkt des AABB, der in Richtung der Ebenen-Normalen am weitesten liegt (p-vertex)
			glm::vec3 p;
			p.x = (plane.normal.x >= 0.0f) ? aabb.max.x : aabb.min.x;
			p.y = (plane.normal.y >= 0.0f) ? aabb.max.y : aabb.min.y;
			p.z = (plane.normal.z >= 0.0f) ? aabb.max.z : aabb.min.z;

			float distP = plane.distance(p);
			if (distP < 0.0f) {
				// ganzes AABB liegt außerhalb dieser Ebene -> kein Schnitt
				return false;
			}
		}
		return true;
	}
};

