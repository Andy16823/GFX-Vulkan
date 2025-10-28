#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

/// <summary>
/// Class representing a 2D camera.
/// </summary>
class Camera2D : public Camera
{
private:
	/// <summary>
	/// The size of the view (width and height).
	/// </summary>
	glm::vec2 m_viewSize;

	/// <summary>
	/// The near clipping plane distance.
	/// </summary>
	float m_near;

	/// <summary>
	/// The far clipping plane distance.
	/// </summary>
	float m_far;

	/// <summary>
	/// Calculates the screen correction factor based on the viewport dimensions.
	/// </summary>
	/// <param name="viewportWidth"></param>
	/// <param name="viewportHeight"></param>
	/// <returns></returns>
	float calculateScreenCorrection(float viewportWidth, float viewportHeight);
public:

	/// <summary>
	/// Creates a new Camera2D instance for 2D rendering.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="viewSize"></param>
	/// <param name=""></param>
	/// <param name=""></param>
	Camera2D(glm::vec2 position, glm::vec2 viewSize, const float near, const float far);

	/// <summary>
	/// Destructor for Camera2D.
	/// </summary>
	~Camera2D() = default;

	/// <summary>
	/// Gets the view matrix for the camera.
	/// </summary>
	/// <returns></returns>
	glm::mat4 getViewMatrix() override;

	/// <summary>
	/// Gets the projection matrix for the camera.
	/// </summary>
	/// <returns></returns>
	glm::mat4 getProjectionMatrix() override;

	/// <summary>
	/// Gets the combined view-projection matrix for the camera.
	/// </summary>
	/// <returns></returns>
	UboViewProjection getViewProjection() override;

	/// <summary>
	/// Moves the camera forward by the specified distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveForward(float distance) override;

	/// <summary>
	/// Moves the camera up by the specified distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveUp(float distance) override;

	/// <summary>
	/// Moves the camera right by the specified distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveRight(float distance) override;

	/// <summary>
	/// Not implemented for 2D camera.
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	void turn(float pitch, float yaw, float roll) override;

	/// <summary>
	/// Projects a world position to screen space.
	/// </summary>
	/// <param name="worldpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	glm::vec3 projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport) override;

	/// <summary>
	/// Projects a screen position back to world space.
	/// </summary>
	/// <param name="screenpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	glm::vec3 unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport) override;
};

