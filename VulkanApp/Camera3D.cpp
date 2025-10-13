#include "Camera3D.h"

Camera3D::Camera3D(glm::vec3 position, glm::vec2 aspectRatio, float fov, float nearPlane, float farPlane)
	: Camera()
{
	m_position = position;
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_fov = fov;
	m_aspectRatio = aspectRatio.x / aspectRatio.y;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

glm::mat4 Camera3D::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera3D::getProjectionMatrix()
{
	glm::mat4 proj = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	proj[1][1] *= -1; // Invert Y for Vulkan
	return proj;
}

UboViewProjection Camera3D::getViewProjection()
{
	UboViewProjection ubo = {};
	ubo.view = getViewMatrix();
	ubo.projection = getProjectionMatrix();
	return ubo;
}

Camera3D::~Camera3D()
{

}