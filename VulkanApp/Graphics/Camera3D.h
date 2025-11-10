#pragma once
#include  "Camera.h"

/// <summary>
/// Class representing a 3D camera.
/// </summary>
class Camera3D
	: public Camera
{
private:
	/// <summary>
	/// The field of view (in degrees).
	/// </summary>
	float m_fov;
	
	/// <summary>
	/// The aspect ratio (width / height).
	/// </summary>
	float m_aspectRatio;

	/// <summary>
	/// The near clipping plane distance.
	/// </summary>
	float m_nearPlane;

	/// <summary>
	/// The far clipping plane distance.
	/// </summary>
	float m_farPlane;
public:

	/// <summary>
	/// Creates a new Camera3D instance for 3D rendering.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="fov"></param>
	/// <param name="nearPlane"></param>
	/// <param name="farPlane"></param>
	Camera3D(glm::vec3 position, glm::vec2 aspectRatio, float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	~Camera3D();

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
	/// Moves the camera forward by the given distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveForward(float distance) override;

	/// <summary>
	/// Moves the camera up by the given distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveUp(float distance) override;

	/// <summary>
	/// Moves the camera right by the given distance.
	/// </summary>
	/// <param name="distance"></param>
	void moveRight(float distance) override;

	/// <summary>
	/// Turns the camera by the given pitch, yaw, and roll (in degrees).
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	void turn(float pitch, float yaw, float roll) override;

	/// <summary>
	/// Sets the far clipping plane distance.
	/// </summary>
	/// <param name="farPlane"></param>
	void setFarPlane(float farPlane) { m_farPlane = farPlane; }

	/// <summary>
	/// Sets the near clipping plane distance.
	/// </summary>
	/// <param name="nearPlane"></param>
	void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

	/// <summary>
	/// Projects a world position to screen space.
	/// </summary>
	/// <param name="worldpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	glm::vec3 projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport) override;

	/// <summary>
	/// Unprojects a screen position to world space.
	/// </summary>
	/// <param name="screenpos"></param>
	/// <param name="viewport"></param>
	/// <returns></returns>
	glm::vec3 unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport) override;

	/// <summary>
	/// Gets the frustum corner points.
	/// </summary>
	/// <returns></returns>
	FrustumPoints& getFrustumPoints() const override;

	/// <summary>
	/// Dumps the camera's information to the console.
	/// </summary>
	void dumpCameraInfo() const override;
};

