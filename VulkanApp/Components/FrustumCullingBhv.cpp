#include "FrustumCullingBhv.h"
#include "../Core/Entity.h"

FrustumCullingBhv::FrustumCullingBhv(Camera* camera)
{
	m_camera = camera;
}

void FrustumCullingBhv::init(Scene* scene, Renderer* renderer)
{
	
}

void FrustumCullingBhv::update(Scene* scene, float dt)
{
	Frustum frustum = m_camera->getFrustum();
	AABB aabb = this->parent->getAABB(true);
	if (aabb.isValid()) {
		if (frustum.intersectsAABB(aabb)) {
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
