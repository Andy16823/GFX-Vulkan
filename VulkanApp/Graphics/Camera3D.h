#pragma once
#include  "Camera.h"
class Camera3D
	: public Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_forward;
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
};

