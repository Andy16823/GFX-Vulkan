#include "Camera3D.h"

Camera3D::Camera3D(glm::vec3 position, glm::vec2 aspectRatio, float fov, float nearPlane, float farPlane)
	: Camera()
{
	transform.position = position;
	m_fov = fov;
	m_aspectRatio = aspectRatio.x / aspectRatio.y;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

glm::mat4 Camera3D::getViewMatrix()
{
	auto cameraForward = transform.position + transform.getForward();
	auto up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(transform.position, cameraForward, up);
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

void Camera3D::moveForward(float distance)
{
	transform.translate(transform.getForward() * distance);
}

void Camera3D::moveUp(float distance)
{
	transform.translate(transform.getUp() * distance);
}

void Camera3D::moveRight(float distance)
{
	auto right = transform.getRight();
	transform.translate(right * distance);
}

void Camera3D::turn(float pitch, float yaw, float roll)
{
	transform.rotate(glm::vec3(pitch, yaw, roll));
}

glm::vec3 Camera3D::projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport)
{
	auto view = getViewMatrix();
	auto proj = getProjectionMatrix();
	glm::vec4 clipSpacePos = proj * view * glm::vec4(worldpos, 1.0f);
	glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;

	glm::vec3 windowPos;
	windowPos.x = ((ndc.x + 1.0f) / 2.0f) * viewport.x;
	windowPos.y = ((1.0f - ndc.y) / 2.0f) * viewport.y;
	windowPos.z = ndc.z;

	return windowPos;
}

glm::vec3 Camera3D::unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport)
{
	auto view = getViewMatrix();
	auto proj = getProjectionMatrix();

	float x = (2.0f * screenpos.x) / viewport.x - 1.0f;
	float y = 1.0f - (2.0f * screenpos.y) / viewport.y;
	float z = screenpos.z * 2.0f - 1.0f;

	glm::vec4 ndc = glm::vec4(x, y, z, 1.0f);
	glm::mat4 invVP = glm::inverse(proj * view);
	glm::vec4 worldPos = invVP * ndc;

	if (worldPos.w != 0.0f) {
		return glm::vec3(worldPos) / worldPos.w;
	}
	return glm::vec3();
}

Camera3D::~Camera3D()
{

}