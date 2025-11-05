#pragma once
#include <glm/glm.hpp>

/// <summary>
/// Axis-Aligned Bounding Box
/// </summary>
struct AABB
{
	/// <summary>
	/// The minimum corner of the AABB
	/// </summary>
	glm::vec3 min;

	/// <summary>
	/// The maximum corner of the AABB
	/// </summary>
	glm::vec3 max;

	/// <summary>
	/// Creates an empty AABB
	/// </summary>
	AABB() : min(glm::vec3(FLT_MAX)), max(glm::vec3(-FLT_MAX)) {}

	/// <summary>
	/// Creates an AABB from min and max points
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

	/// <summary>
	/// Adds an offset to the AABB and returns a new AABB
	/// </summary>
	/// <param name="offset"></param>
	/// <returns></returns>
	AABB operator +(const glm::vec3& offset) const
	{
		return AABB(min + offset, max + offset);
	}

	/// <summary>
	/// Subtracts an offset from the AABB and returns a new AABB
	/// </summary>
	AABB operator -(const glm::vec3& offset) const
	{
		return AABB(min - offset, max - offset);
	}

	/// <summary>
	/// Applies a scale to the AABB and returns a new AABB
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	AABB operator *(const glm::vec3& scale) const
	{
		return AABB(min * scale, max * scale);
	}

	/// <summary>
	/// Applies a uniform scale to the AABB and returns a new AABB
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	AABB operator *(float scale) const
	{
		return AABB(min * scale, max * scale);
	}

	/// <summary>
	/// Applies an inverse scale to the AABB and returns a new AABB
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	AABB operator /(const glm::vec3& scale) const
	{
		return AABB(min / scale, max / scale);
	}

	/// <summary>
	/// Applies a uniform scale to the AABB and returns a new AABB
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	AABB operator /(float scale) const
	{
		return AABB(min / scale, max / scale);
	}

	/// <summary>
	/// Adds another AABB to this AABB and returns a new AABB that encompasses both
	/// </summary>
	AABB operator +(const AABB& other) const
	{
		return AABB(glm::min(min, other.min), glm::max(max, other.max));
	}

	/// <summary>
	/// Combines another AABB into this AABB
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	AABB& operator +=(const AABB& other)
	{
		min = glm::min(min, other.min);
		max = glm::max(max, other.max);
		return *this;
	}

	/// <summary>
	/// Subtracts another AABB from this AABB and returns a new AABB that is the overlapping region
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	AABB operator -(const AABB& other) const
	{
		return AABB(glm::max(min, other.min), glm::min(max, other.max));
	}

	/// <summary>
	/// Combines another AABB into this AABB as the overlapping region
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	AABB& operator -=(const AABB& other)
	{
		min = glm::max(min, other.min);
		max = glm::min(max, other.max);
		return *this;
	}

	/// <summary>
	/// Applies a transformation matrix to the AABB and returns a new AABB
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	AABB operator *(const glm::mat4& value) {
		AABB result;
		result.min = glm::vec3(value * glm::vec4(min, 1.0f));
		result.max = glm::vec3(value * glm::vec4(max, 1.0f));
		return result;
	}

	/// <summary>
	/// Compares two AABBs for equality
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const AABB& other) const
	{
		return min == other.min && max == other.max;
	}


	/// <summary>
	/// Expands the AABB to include another AABB
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	AABB& expand(const AABB& other)
	{
		min = glm::min(min, other.min);
		max = glm::max(max, other.max);
		return *this;
	}

	/// <summary>
	/// Expands the AABB to include the given point
	/// </summary>
	/// <param name="point"></param>
	void expand(const glm::vec3& point)
	{
		min = glm::min(min, point);
		max = glm::max(max, point);
	}
};

