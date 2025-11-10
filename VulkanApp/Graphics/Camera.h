#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Utils.h"
#include "../Math/Transform.h"
#include "../Math/Frustum.h"


/// <summary>
/// Base class representing a camera.
/// </summary>
class Camera
{
private:
	/// <summary>
	/// The frustum associated with the camera.
	/// </summary>
	Frustum m_frustum;

public:

	Camera() = default;
	virtual ~Camera() = default;

	/// <summary>
	/// Gets the frustum associated with the camera.
	/// </summary>
	/// <returns></returns>
	Frustum& getFrustum() { return m_frustum; }

	/// <summary>
	/// Creates the frustum for the camera.
	/// </summary>
	void createFrustum();

	/// <summary>
	/// Updates the camera's frustum.
	/// </summary>
	void update();

	/// <summary>
	/// Transform of the camera (position, rotation, scale).
	/// </summary>
	Transform transform;

	/// <summary>
	/// Gets the view matrix for the camera.
	/// </summary>
	/// <returns></returns>
	virtual glm::mat4 getViewMatrix() = 0;

	/// <summary>
	/// Gets the projection matrix for the camera.
	/// </summary>
	/// <returns></returns>
	virtual glm::mat4 getProjectionMatrix() = 0;

	/// <summary>
	/// Gets the combined view-projection matrix for the camera.
	/// </summary>
	/// <returns></returns>
	virtual UboViewProjection getViewProjection() = 0;

	/// <summary>
	/// Moves the camera forward by the specified distance.
	/// </summary>
	/// <param name="distance"></param>
	virtual void moveForward(float distance) = 0;

	/// <summary>
	/// Moves the camera up by the given distance.
	/// </summary>
	/// <param name="distance"></param>
	virtual void moveUp(float distance) = 0;

	/// <summary>
	/// Moves the camera right by the given distance.
	/// </summary>
	/// <param name="distance"></param>
	virtual void moveRight(float distance) = 0;

	/// <summary>
	/// Turns the camera by the given pitch, yaw, and roll angles.
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	virtual void turn(float pitch, float yaw, float roll) = 0;

	/// <summary>
	/// Projects a world position to screen space.
	/// </summary>
	/// <param name="worldpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	virtual glm::vec3 projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport) = 0;

	/// <summary>
	/// Projects a screen position back to world space.
	/// </summary>
	/// <param name="screenpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	virtual glm::vec3 unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport) = 0;

	/// <summary>
	/// Gets the frustum points of the camera.
	/// </summary>
	/// <returns></returns>
	virtual FrustumPoints& getFrustumPoints() const = 0;

	/// <summary>
	/// Dumps camera information for debugging purposes.
	/// </summary>
	virtual void dumpCameraInfo() const = 0;
};

