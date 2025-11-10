#pragma once
#include  "Camera.h"
class Camera3D
	: public Camera
{
private:
	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
public:
	Camera3D(glm::vec3 position, glm::vec2 aspectRatio, float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	~Camera3D();

	glm::mat4 getViewMatrix() override;
	glm::mat4 getProjectionMatrix() override;
	UboViewProjection getViewProjection() override;

	void moveForward(float distance) override;
	void moveUp(float distance) override;
	void moveRight(float distance) override;
	void turn(float pitch, float yaw, float roll) override;

	void setFarPlane(float farPlane) { m_farPlane = farPlane; }
	void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

	glm::vec3 projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport) override;
	glm::vec3 unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport) override;

	Frustum getFrustum() const override;

	void dumpCameraInfo() const override;

};

