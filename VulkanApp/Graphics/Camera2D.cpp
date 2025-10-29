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

Camera2D::Camera2D(glm::vec3 position, glm::vec2 viewSize, const float near, const float far)
{
	m_near = near;
	m_far = far;
	this->transform.position = position;
	this->m_viewSize = viewSize;
}

glm::mat4 Camera2D::getViewMatrix()
{
	return glm::lookAt(transform.position, transform.position + transform.getForward(), transform.getUp());
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//return view;
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

	auto projection = glm::ortho(left, right, bottom, top, m_near, m_far);

	// Vulkan Fix: Invertiere die Y-Achse
	projection[1][1] *= -1;
	return projection;
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

glm::vec3 Camera2D::projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport)
{
	glm::vec4 clipSpacePos = getProjectionMatrix() * getViewMatrix() * glm::vec4(worldpos, 1.0f);
	glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;
	glm::vec3 windowPos;
	windowPos.x = ((ndc.x + 1.0f) / 2.0f) * viewport.x;
	windowPos.y = ((1.0f - ndc.y) / 2.0f) * viewport.y;
	windowPos.z = ndc.z;
	return windowPos;
}

glm::vec3 Camera2D::unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport)
{
	float ndcX = (2.0f * screenpos.x) / viewport.x - 1.0f;
	float ndcY = 1.0f - (2.0f * screenpos.y) / viewport.y;
	float ndcZ = screenpos.z * 2.0f - 1.0f;

	glm::vec4 ndc = glm::vec4(ndcX, ndcY, ndcZ, 1.0f);
	glm::mat4 invVP = glm::inverse(getProjectionMatrix() * getViewMatrix());
	glm::vec4 worldPos = invVP * ndc;
	return glm::vec3(worldPos) / worldPos.w;
}
