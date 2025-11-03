#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "InstancedModel.h"
#include "../Assets/ModelLoader.h"

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, int instances) : Entity(name)
{
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED);
	m_meshResource = ressource;
	this->instanceCount = instances;
}

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, const std::vector<InstanceData>& startValues, int instances)
	: Entity(name)
{
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED);
	m_meshResource = ressource;
	this->instanceCount = instances;
	if (startValues.size() > instances) {
		throw std::runtime_error("failed to create instanced model: start values size exceeds instance count!");
	}
	m_instanceStartValues = startValues;
}

void InstancedModel::init(Scene* scene, Renderer* renderer)
{
	// Calculate the size of the storage buffer and create it
	VkDeviceSize bufferSize = sizeof(InstanceData) * instanceCount;
	m_storageBufferIndex = renderer->createStorageBuffer(bufferSize);

	// Update the initial instance data if provided
	if (!m_instanceStartValues.empty()) {
		this->updateInstanceRange(renderer, m_instanceStartValues, 0);
		m_instanceStartValues.clear();
	}
}

void InstancedModel::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	// Early out if pipeline type is not set
	if (this->pipelineType.empty()) {
		throw std::runtime_error("failed to render instanced model: pipeline type is not set!");
	}

	// Validate the mesh resource
	if (!m_meshResource) {
		throw std::runtime_error("failed to render instanced model: mesh resource is null!");
	}

	// Early out if no instances or no meshes
	if (instanceCount == 0 || m_meshResource->meshes.empty()) {
		return;
	}

	// Get the storage buffer for the instance data and the active camera
	auto storageBuffer = renderer->getStorageBuffer(m_storageBufferIndex);
	auto camera = renderer->getActiveCamera();

	// Bind the pipeline to render with
	renderer->bindPipeline(commandBuffer, this->pipelineType);

	// Bind the scene descriptor sets (e.g. lights)
	scene->bindSceneDescriptorSets(renderer, commandBuffer, currentFrame, this->pipelineType);

	// Bind the model related push constants
	VkDescriptorSet cameraDescriptorSet = renderer->getCameraDescriptorSet(camera, currentFrame);
	VkDescriptorSet storageBufferDescriptorSet = renderer->getStorageBufferDescriptorSet(storageBuffer->descriptorIndex);
	std::array<VkDescriptorSet, 2> baseDescriptorSets = {
		cameraDescriptorSet,
		storageBufferDescriptorSet
	};
	renderer->bindDescriptorSets(baseDescriptorSets, 0, currentFrame);

	// Render each mesh in the model
	for (auto& mesh : m_meshResource->meshes) {
		auto material = mesh->material.get();

		// Bind the material related descriptor sets
		material->bindMaterial(renderer, commandBuffer, 2, currentFrame);

		// Draw the mesh with instancing
		renderer->drawBuffers(mesh->vertexBufferIndex, mesh->indexBufferIndex, commandBuffer, instanceCount);
	}
}

void InstancedModel::destroy(Scene* scene, Renderer* renderer)
{
	// Nothing needed here as the renderer handles buffer cleanup and the asset manager handles mesh cleanup
}

void InstancedModel::updateInstance(Renderer* renderer, const InstanceData& data, int instanceIndex)
{
	if (instanceIndex < 0 || instanceIndex >= instanceCount) {
		throw std::runtime_error("failed to update instance data: invalid instance index!");
	}

	VkDeviceSize size = sizeof(InstanceData);
	VkDeviceSize offset = sizeof(InstanceData) * instanceIndex;
	renderer->updateStorageBuffer(m_storageBufferIndex, &data, size, offset);
}

void InstancedModel::updateInstanceRange(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray, int offset)
{
	int instanceCount = static_cast<int>(instanceDataArray.size());
	if (offset < 0 || (offset + instanceCount) > this->instanceCount) {
		throw std::runtime_error("failed to update instance range data: invalid offset or instance count!");
	}
	VkDeviceSize size = sizeof(InstanceData) * instanceCount;
	VkDeviceSize bufferOffset = sizeof(InstanceData) * offset;
	renderer->updateStorageBuffer(m_storageBufferIndex, instanceDataArray.data(), size, bufferOffset);
}

void InstancedModel::updateAllInstances(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray)
{
	if (instanceDataArray.size() != instanceCount) {
		throw std::runtime_error("failed to update all instance data: instance data array size does not match instance count!");
	}
	VkDeviceSize size = sizeof(InstanceData) * instanceCount;
	renderer->updateStorageBuffer(m_storageBufferIndex, instanceDataArray.data(), size, 0);
}