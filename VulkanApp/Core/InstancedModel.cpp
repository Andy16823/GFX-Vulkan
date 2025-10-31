#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "InstancedModel.h"
#include "../Assets/ModelLoader.h"

InstancedModel::InstancedModel(const std::string name, const std::string file, const int instances) : Entity(name)
{
	m_meshes = ModelLoader::loadModelFromFile(file);
	this->instanceCount = instances;
}

void InstancedModel::init(Renderer* renderer)
{
	// Calculate the size of the storage buffer and create it
	VkDeviceSize bufferSize = sizeof(InstanceData) * instanceCount;
	m_storageBufferIndex = renderer->createStorageBuffer(bufferSize);

	for (auto& mesh : m_meshes) {
		mesh.get()->init(renderer);
	}
}

void InstancedModel::render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	// Early out if there are no instances or no meshes
	if (instanceCount == 0 || m_meshes.empty()) {
		return;
	}

	// Get the required buffers wich all meshes share
	auto storageBuffer = renderer->getStorageBuffer(m_storageBufferIndex);
	auto camera = renderer->getActiveCamera();
	VkDescriptorSet cameraDescriptorSet = renderer->getCameraDescriptorSet(camera, currentFrame);
	VkDescriptorSet storageBufferDescriptorSet = renderer->getStorageBufferDescriptorSet(storageBuffer->descriptorIndex);
	renderer->bindPipeline(commandBuffer, ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED));

	// Iterate through all meshes and render them
	for (auto& mesh : m_meshes) {
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
	for (auto& mesh : m_meshes) {
		mesh.get()->dispose(renderer);
	}
}

void InstancedModel::updateInstanceData(Renderer* renderer, const InstanceData& data, int instanceIndex)
{
	if (instanceIndex < 0 || instanceIndex >= instanceCount) {
		throw std::runtime_error("failed to update instance data: invalid instance index!");
	}

	VkDeviceSize size = sizeof(InstanceData);
	VkDeviceSize offset = sizeof(InstanceData) * instanceIndex;
	renderer->updateStorageBuffer(m_storageBufferIndex, &data, size, offset);
}
