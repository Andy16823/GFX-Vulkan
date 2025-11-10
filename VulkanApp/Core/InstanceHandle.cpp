#include "InstanceHandle.h"
#include "Instancer.h"

bool InstanceHandle::isDirty()
{
	if (m_previousState != this->getState()) 
	{
		m_previousState = this->getState();
		return true;
	}
	return false;
}

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

	if(this->isDirty() == false)
	{
		return;
	}
	// Only update instance data if something changed
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

void InstanceHandle::update(Scene* scene, float dt)
{
	if (this->hasState(EntityState::ENTITY_STATE_ACTIVE)) 
	{
		Entity::update(scene, dt);
	}
}

void InstanceHandle::createAABB()
{
	auto modelAABB = m_instancer->getAABB(false);
}
