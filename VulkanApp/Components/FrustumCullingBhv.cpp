#include "FrustumCullingBhv.h"
#include "../Core/Entity.h"

bool FrustumCullingBhv::isDirty(Transform& transform)
{
	if(transform.position != m_lastParentPosition ||
		transform.rotation != m_lastParentRotation ||
		transform.scale != m_lastParentScale)
	{
		m_lastParentPosition = transform.position;
		m_lastParentRotation = transform.rotation;
		m_lastParentScale = transform.scale;
		return true;
	}
	return false;
}

FrustumCullingBhv::FrustumCullingBhv(Camera* camera, CullingMode mode)
{
	m_camera = camera;
	m_cullingMode = mode;
}

void FrustumCullingBhv::init(Scene* scene, Renderer* renderer)
{

}

void FrustumCullingBhv::update(Scene* scene, float dt)
{
	// Get the AABB if the transform is dirty
	if (isDirty(this->parent->transform))
	{
		m_aabb = this->parent->getAABB(true);
	}

	// Perform frustum culling
	auto frustum = m_camera->getFrustum();
	float radius;

	switch (m_cullingMode)
	{
	case CullingMode::AABB_CULLING:
		if (m_aabb.isValid()) {
			if (frustum.intersectsAABB(m_aabb)) {
				this->parent->addState(EntityState::ENTITY_STATE_VISIBLE);
			}
			else {
				this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
			}
		}
		break;
	case CullingMode::SPHERE_CULLING:
		radius = glm::length(m_aabb.halfExtents());
		if (frustum.intersectsSphere(m_aabb.center(), radius)) {
			this->parent->addState(EntityState::ENTITY_STATE_VISIBLE);
		}
		else {
			this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
		}
		break;
	case CullingMode::SPHERE_THEN_AABB_CULLING:
		radius = glm::length(m_aabb.halfExtents());
		if (frustum.intersectsSphere(m_aabb.center(), radius)) {
			if(frustum.intersectsAABB(m_aabb)) {
				this->parent->addState(EntityState::ENTITY_STATE_VISIBLE);
			}
			else {
				this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
			}
		}
		else {
			this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
		}
		break;
	case CullingMode::ORIGIN_CULLING:
		if (frustum.containsPoint(m_aabb.center())) {
			this->parent->addState(EntityState::ENTITY_STATE_VISIBLE);
		}
		else {
			this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
		}
		break;
	default:
		break;
	}	
}

void FrustumCullingBhv::destroy(Scene* scene, Renderer* renderer)
{
	
}

void FrustumCullingBhv::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame)
{
	
}

std::string FrustumCullingBhv::getIdentifier()
{
	return "FrustumCullingBhv";
}
