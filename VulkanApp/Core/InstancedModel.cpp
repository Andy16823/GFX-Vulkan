#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "InstancedModel.h"
#include "../Assets/ModelLoader.h"

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, int instances) 
	: Instancer(name, instances)
{
	this->name = name;
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED);
	m_meshResource = ressource;
}

InstancedModel::InstancedModel(const std::string& name, StaticMeshesRsc* ressource, const std::vector<InstanceData>& startValues, int instances)
	: Instancer(name, startValues, instances)
{
	this->name = name;
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D_INSTANCED);
	m_meshResource = ressource;
}

void InstancedModel::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	if (this->hasState(EntityState::ENTITY_STATE_VISIBLE)) 
	{
		Entity::render(scene, renderer, commandBuffer, currentFrame);

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
		auto storageBuffer = renderer->getStorageBuffer(this->getStorageBufferIndex());
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
}

void InstancedModel::update(Scene* scene, float dt)
{
	if(this->hasState(EntityState::ENTITY_STATE_ACTIVE)) 
	{
		Entity::update(scene, dt);
	}
}

void InstancedModel::createAABB()
{
	AABB aabb;
	for (const auto& mesh : m_meshResource->meshes) {
		for (const auto& vertex : mesh->vertices) 
		{
			aabb.expand(vertex.pos);
		}
	}
	this->setAABB(aabb);
}

