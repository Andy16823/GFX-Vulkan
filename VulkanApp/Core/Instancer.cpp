#include "Instancer.h"

Instancer::Instancer(const std::string& name, int instances) 
	: Entity(name)
{
	this->instanceCount = instances;
}

Instancer::Instancer(const std::string& name, const std::vector<InstanceData>& startValues, int instances) 
	: Entity(name)
{
	if (startValues.size() > instances) {
		throw std::runtime_error("failed to create instanced model: start values size exceeds instance count!");
	}
	m_instanceStartValues = startValues;
	this->instanceCount = instances;
}

void Instancer::init(Scene* scene, Renderer* renderer)
{
	VkDeviceSize bufferSize = sizeof(InstanceData) * instanceCount;
	m_storageBufferIndex = renderer->createStorageBuffer(bufferSize);

	// Update the initial instance data if provided
	if (!m_instanceStartValues.empty()) {
		this->updateInstanceRange(renderer, m_instanceStartValues, 0);
		m_instanceStartValues.clear();
	}
}

std::unique_ptr<InstanceHandle> Instancer::createInstanceHandle(const std::string& name, int instanceId)
{
	std::unique_ptr<InstanceHandle> handle = std::make_unique<InstanceHandle>(name, this, instanceId);
	handle->setAABB(this->getAABB(false));
	return handle;
}

void Instancer::updateInstance(Renderer* renderer, const InstanceData& data, int instanceIndex)
{
	if (instanceIndex < 0 || instanceIndex >= instanceCount) {
		throw std::runtime_error("failed to update instance data: invalid instance index!");
	}

	VkDeviceSize size = sizeof(InstanceData);
	VkDeviceSize offset = sizeof(InstanceData) * instanceIndex;
	renderer->updateStorageBuffer(m_storageBufferIndex, &data, size, offset);
}

void Instancer::updateInstanceRange(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray, int offset)
{
	int updateInstanceCount = static_cast<int>(instanceDataArray.size());
	if (offset < 0 || (offset + updateInstanceCount) > this->instanceCount) {
		throw std::runtime_error("failed to update instance range data: invalid offset or instance count!");
	}

	VkDeviceSize size = sizeof(InstanceData) * updateInstanceCount;
	VkDeviceSize bufferOffset = sizeof(InstanceData) * offset;
	renderer->updateStorageBuffer(m_storageBufferIndex, instanceDataArray.data(), size, bufferOffset);
}

void Instancer::updateAllInstances(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray)
{
	if (instanceDataArray.size() != instanceCount) {
		throw std::runtime_error("failed to update all instance data: instance data array size does not match instance count!");
	}
	VkDeviceSize size = sizeof(InstanceData) * instanceCount;
	renderer->updateStorageBuffer(m_storageBufferIndex, instanceDataArray.data(), size, 0);
}