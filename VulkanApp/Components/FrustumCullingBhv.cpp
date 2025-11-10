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
		std::cout << "Entity " << this->parent->name << " was dirty. Update AABB" << std::endl;
	}

	auto frustum = m_camera->getFrustum();
	if (m_aabb.isValid()) {
		if (frustum.intersectsAABB(m_aabb)) {
			this->parent->addState(EntityState::ENTITY_STATE_VISIBLE);
		}
		else {
			this->parent->removeState(EntityState::ENTITY_STATE_VISIBLE);
		}
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
