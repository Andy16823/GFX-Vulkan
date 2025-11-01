#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "InstancedModel.h"
#include "../Assets/ModelLoader.h"

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, int instances) : Entity(name)
{
	m_meshResource = ressource;
	this->instanceCount = instances;
}

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, const std::vector<InstanceData>& startValues, int instances)
	: Entity(name)
{
	m_meshResource = ressource;
	this->instanceCount = instances;
	if (startValues.size() > instances) {
		throw std::runtime_error("failed to create instanced model: start values size exceeds instance count!");
	}
	m_instanceStartValues = startValues;
}

void InstancedModel::init(Renderer* renderer)
{
	VkDeviceSize bufferSize = sizeof(InstanceData) * instanceCount;
	m_storageBufferIndex = renderer->createStorageBuffer(bufferSize);

	// If start values are provided, initialize the buffer with them
	if (!m_instanceStartValues.empty()) {
		this->updateInstanceRange(renderer, m_instanceStartValues, 0);
		m_instanceStartValues.clear(); // Clear to free memory
	}
}

void InstancedModel::render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	// Ensure the mesh resource is valid
	if (!m_meshResource) {
		throw std::runtime_error("failed to render instanced model: mesh resource is null!");
	}

	// Early out if there are no instances or no meshes
	if (instanceCount == 0 || m_meshResource->meshes.empty()) {
		return;
	}

	// Get the required buffers wich all meshes share
	auto storageBuffer = renderer->getStorageBuffer(m_storageBufferIndex);
	auto camera = renderer->getActiveCamera();
	VkDescriptorSet cameraDescriptorSet = renderer->getCameraDescriptorSet(camera, currentFrame);
	VkDescriptorSet storageBufferDescriptorSet = renderer->getStorageBufferDescriptorSet(storageBuffer->descriptorIndex);
	renderer->bindPipeline(commandBuffer, ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED));

	// Iterate through all meshes and render them
	for (auto& mesh : m_meshResource->meshes) {
		std::vector<VkDescriptorSet> descriptorSets;
		descriptorSets.push_back(cameraDescriptorSet); // Add the per-frame UBO descriptor set first
		descriptorSets.push_back(storageBufferDescriptorSet); // Add the storage buffer descriptor set second

		// Get the texture descriptor sets from the material
		auto material = mesh->material.get();
		auto textureBufferIndices = material->getTextureIndices();
		for (size_t i = 0; i < textureBufferIndices.size(); i++) {
			int imageBufferIndex = textureBufferIndices[i];
			auto imageBuffer = renderer->getImageBuffer(imageBufferIndex);
			VkDescriptorSet textureDescriptorSet = renderer->getSamplerDescriptorSet(imageBuffer->descriptorIndex);
			descriptorSets.push_back(textureDescriptorSet);
		}
		renderer->bindDescriptorSets(descriptorSets, currentFrame);
		renderer->drawBuffers(mesh->vertexBufferIndex, mesh->indexBufferIndex, commandBuffer, instanceCount);
	}
}

void InstancedModel::destroy(Renderer* renderer)
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