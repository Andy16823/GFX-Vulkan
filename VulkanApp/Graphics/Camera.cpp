#include "Camera.h"

void Camera::createFrustum()
{
	FrustumPoints frustumPoints = this->getFrustumPoints();
	m_frustum = Frustum(frustumPoints);
}

void Camera::update()
{
	FrustumPoints frustumPoints = this->getFrustumPoints();
	m_frustum.update(frustumPoints);
}
