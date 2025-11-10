#include "Camera3D.h"

Camera3D::Camera3D(glm::vec3 position, glm::vec2 aspectRatio, float fov, float nearPlane, float farPlane)
	: Camera()
{
	transform.position = position;
	m_fov = fov;
	m_aspectRatio = aspectRatio.x / aspectRatio.y;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	this->createFrustum();
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
	ubo.cameraPos = this->transform.position;
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

	// GLM's eingebaute unProject Funktion verwenden
	glm::vec4 viewportVec = glm::vec4(0.0f, 0.0f, viewport.x, viewport.y);
	return glm::unProject(screenpos, view, proj, viewportVec);
}


FrustumPoints& Camera3D::getFrustumPoints() const
{
	// Direction Vectors
	glm::vec3 forward = transform.getForward();
	glm::vec3 right = transform.getRight();
	glm::vec3 up = transform.getUp();

	// Near and Far plane distances
	float neardist = m_nearPlane;
	float fardist = m_farPlane;
	float fovY = glm::radians(m_fov);

	// Centers of Near and Far planes
	glm::vec3 nearCenter = transform.position + forward * neardist;
	glm::vec3 farCenter = transform.position + forward * fardist;

	// Calculate height and width of Near and Far planes
	float nearHeigth = 2.0f * tan(fovY * 0.5f) * neardist;
	float nearWidth = nearHeigth * m_aspectRatio;

	float farHeigth = 2.0f * tan(fovY * 0.5f) * fardist;
	float farWidth = farHeigth * m_aspectRatio;

	// Half dimensions
	float halfNearWidth = nearWidth / 2.0f;
	float halfNearHeigth = nearHeigth / 2.0f;
	float halfFarWidth = farWidth / 2.0f;
	float halfFarHeigth = farHeigth / 2.0f;

	// Frustum corners
	FrustumPoints frustumPoints;

	// Near plane corners
	frustumPoints.nearTopLeft = nearCenter + (up * halfNearHeigth) - (right * halfNearWidth);
	frustumPoints.nearTopRight = nearCenter + (up * halfNearHeigth) + (right * halfNearWidth);
	frustumPoints.nearBottomLeft = nearCenter - (up * halfNearHeigth) - (right * halfNearWidth);
	frustumPoints.nearBottomRight = nearCenter - (up * halfNearHeigth) + (right * halfNearWidth);

	// Far plane corners
	frustumPoints.farTopLeft = farCenter + (up * halfFarHeigth) - (right * halfFarWidth);
	frustumPoints.farTopRight = farCenter + (up * halfFarHeigth) + (right * halfFarWidth);
	frustumPoints.farBottomLeft = farCenter - (up * halfFarHeigth) - (right * halfFarWidth);
	frustumPoints.farBottomRight = farCenter - (up * halfFarHeigth) + (right * halfFarWidth);

	return frustumPoints;
}

void Camera3D::dumpCameraInfo() const
{
	glm::vec3 forward = transform.getForward();
	glm::vec3 right = transform.getRight();
	glm::vec3 up = transform.getUp();

	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 right_test = glm::normalize(glm::cross(forward, worldUp));
	glm::vec3 up_test = glm::normalize(glm::cross(right_test, forward));

	printf("forward: %.3f %.3f %.3f\n", forward.x, forward.y, forward.z);
	printf("right:   %.3f %.3f %.3f\n", right.x, right.y, right.z);
	printf("up:      %.3f %.3f %.3f\n", up.x, up.y, up.z);
	printf("right_test (f x worldUp): %.3f %.3f %.3f\n", right_test.x, right_test.y, right_test.z);
	printf("up_test    (r_test x f):  %.3f %.3f %.3f\n", up_test.x, up_test.y, up_test.z);

	printf("dot(forward,right)=%.3f dot(forward,up)=%.3f dot(right,up)=%.3f\n",
		glm::dot(forward, right), glm::dot(forward, up), glm::dot(right, up));
}

Camera3D::~Camera3D()
{

}