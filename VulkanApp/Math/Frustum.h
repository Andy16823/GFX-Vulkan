#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "Plane.h"
#include "AABB.h"
#include <vector>
#include <array>

/// <summary>
/// Frustum corner points structure
/// </summary>
struct FrustumPoints
{
	glm::vec3 nearTopLeft;
	glm::vec3 nearTopRight;
	glm::vec3 nearBottomLeft;
	glm::vec3 nearBottomRight;
	glm::vec3 farTopLeft;
	glm::vec3 farTopRight;
	glm::vec3 farBottomLeft;
	glm::vec3 farBottomRight;
};

/// <summary>
/// Frustum class representing a view frustum defined by six planes
/// </summary>
class Frustum
{
public:
	/// <summary>
	/// Standard constructor
	/// </summary>
	Frustum() = default;

	/// <summary>
	/// Frustum constructor with corner points
	/// </summary>
	/// <param name="pts"></param>
	Frustum(const FrustumPoints& pts) : points(pts) {
		this->computePlanes();
	}
	~Frustum() = default;
	
	/// <summary>
	/// The corner points of the frustum
	/// </summary>
	FrustumPoints points;

	/// <summary>
	/// The six planes of the frustum
	/// </summary>
	std::array<Plane, 6> planes;



	/// <summary>
	/// Updates the frustum with new corner points
	/// </summary>
	/// <param name="pts"></param>
	void update(const FrustumPoints& pts) {
		this->points = pts;
		this->computePlanes();
	}

	/// <summary>
	/// Gets the center point of the frustum
	/// </summary>
	/// <returns></returns>
	glm::vec3 center() const {
		glm::vec3 center = (points.nearTopLeft + points.nearTopRight + points.nearBottomLeft + points.nearBottomRight +
			points.farTopLeft + points.farTopRight + points.farBottomLeft + points.farBottomRight) * (1.0f / 8.0f);
		return center;
	}

	/// <summary>
	/// Computes the six planes of the frustum from the corner points
	/// </summary>
	void computePlanes() {
		std::array<Plane, 6> planes;
		planes[0] = Plane::fromPoints(points.nearTopLeft, points.nearTopRight, points.nearBottomRight);      // Near
		planes[1] = Plane::fromPoints(points.farTopRight, points.farTopLeft, points.farBottomLeft);          // Far
		planes[2] = Plane::fromPoints(points.nearTopLeft, points.farTopLeft, points.farBottomLeft);          // Left
		planes[3] = Plane::fromPoints(points.nearTopRight, points.farTopRight, points.farBottomRight);       // Right
		planes[4] = Plane::fromPoints(points.nearTopLeft, points.farTopLeft, points.farTopRight);            // Top
		planes[5] = Plane::fromPoints(points.nearBottomLeft, points.farBottomLeft, points.farBottomRight);   // Bottom

		// Inneren Bezugspunkt (Mittelpunkt der 8 Ecken)
		glm::vec3 center = this->center();

		// Drehe jede Ebene, falls center auf negativer Seite liegt -> dann ist center dist < 0
		for (auto& pl : planes) {
			if (pl.distance(center) < 0.0f) {
				pl.normal = -pl.normal;
				pl.d = -pl.d;
			}
		}

		this->planes = planes;
	}


	/// <summary>
	/// Checks if the frustum intersects with the given Axis-Aligned Bounding Box (AABB)
	/// </summary>
	/// <param name="aabb"></param>
	/// <returns></returns>
	bool intersectsAABB(const AABB& aabb) const {
		return intersectsAABB(aabb, this->planes);
	}

	static bool intersectsAABB(const AABB& aabb, const std::array<Plane, 6>& planes) {
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

	/// <summary>
	/// Checks if the frustum contains the given point
	/// </summary>
	/// <param name="point"></param>
	/// <returns></returns>
	bool containsPoint(const glm::vec3& point) const {
		return containsPoint(point, this->planes);
	}

	/// <summary>
	/// Checks if the frustum contains the given point against the provided planes
	/// </summary>
	/// <param name="point"></param>
	/// <param name="planes"></param>
	/// <returns></returns>
	static bool containsPoint(const glm::vec3& point, const std::array<Plane, 6>& planes) {
		for (const auto& plane : planes) {
			if (plane.distance(point) < 0.0f) {
				return false;
			}
		}
		return true;
	}

	/// <summary>
	/// Checks if the frustum contains the given sphere
	/// </summary>
	/// <param name="center"></param>
	/// <param name="radius"></param>
	/// <returns></returns>
	bool containsSphere(const glm::vec3& center, float radius) const {
		return containsSphere(center, radius, this->planes);
	}

	/// <summary>
	/// Checks if the frustum contains the given sphere against the provided planes
	/// </summary>
	/// <param name="center"></param>
	/// <param name="radius"></param>
	/// <param name="planes"></param>
	/// <returns></returns>
	static bool containsSphere(const glm::vec3& center, float radius, const std::array<Plane, 6>& planes) {
		for (const auto& plane : planes) {
			if (plane.distance(center) < -radius) {
				return false;
			}
		}
		return true;
	}
};
