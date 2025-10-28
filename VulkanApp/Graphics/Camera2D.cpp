#include "Camera2D.h"

float Camera2D::calculateScreenCorrection(float viewportWidth, float viewportHeight)
{
	float vpAspectRatio = viewportWidth / viewportHeight;
	float viewAspectRatio = m_viewSize.x / m_viewSize.y;
	if (vpAspectRatio > viewAspectRatio) {
		return vpAspectRatio / viewAspectRatio;
	}
	else if (vpAspectRatio < viewAspectRatio) {
		return viewAspectRatio / vpAspectRatio;
	}
}

Camera2D::Camera2D(glm::vec2 position, glm::vec2 viewSize, const float near, const float far)
{
	m_near = near;
	m_far = far;
	this->transform.position = glm::vec3(position, 0.0f);
	this->m_viewSize = viewSize;
}

glm::mat4 Camera2D::getViewMatrix()
{
	return glm::lookAt(
		transform.position,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
}

glm::mat4 Camera2D::getProjectionMatrix()
{
	//float correction = calculateScreenCorrection(m_viewSize.x, m_viewSize.y); // 1.0f for no correction yet
	// float halfWidth = (viewport.Width / 2) / correction;
	// float halfHeight = (viewport.Height / 2) / correction;
	
	float halfWidth = m_viewSize.x / 2.0f;
	float halfHeight = m_viewSize.y / 2.0f;

	float left = this->transform.position.x - halfWidth;
	float right = this->transform.position.x + halfWidth;
	float bottom = this->transform.position.y - halfHeight;
	float top = this->transform.position.y + halfHeight;

	return glm::ortho(left, right, bottom, top, m_near, m_far);
}

UboViewProjection Camera2D::getViewProjection()
{
	UboViewProjection viewProj = {};
	viewProj.projection = getProjectionMatrix();
	viewProj.view = getViewMatrix();
	return viewProj;
}

void Camera2D::moveForward(float distance)
{
	this->transform.position.z += distance;
}

void Camera2D::moveUp(float distance)
{
	this->transform.position.y += distance;
}

void Camera2D::moveRight(float distance)
{
	this->transform.position.x += distance;
}

void Camera2D::turn(float pitch, float yaw, float roll)
{
	// No rotation for 2D camera
}
