#include "InstanceHandle.h"
#include "Instancer.h"

InstanceHandle::InstanceHandle(const std::string& name, Instancer* model, int instanceId) 
	: Entity(name)
{
	this->m_instancer = model;
	this->instanceID = instanceId;
}

void InstanceHandle::init(Scene* scene, Renderer* renderer)
{
	
}

void InstanceHandle::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	Entity::render(scene, renderer, commandBuffer, currentFrame);

	InstanceData data = {};
	data.model = this->transform.getMatrix();
	if (this->hasState(EntityState::ENTITY_STATE_VISIBLE))
	{
		data.extras.x = 1.0f;
	}
	else {
		data.extras.x = 0.0f;
	}
	m_instancer->updateInstance(renderer, data, instanceID);
}

void InstanceHandle::destroy(Scene* scene, Renderer* renderer)
{
	
}

void InstanceHandle::createAABB()
{
	auto modelAABB = m_instancer->getAABB(false);
}
