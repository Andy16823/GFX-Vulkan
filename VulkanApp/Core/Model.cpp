#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/PBRMaterial.h"
#include "../Assets/ModelLoader.h"


Model::Model(std::string name, StaticMeshesRsc* ressource) 
	: Entity(name)
{
	this->pipelineType = ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D);
	m_meshResource = ressource;
}

void Model::update(Scene* scene, float dt)
{
	Entity::update(scene, dt);
}


void Model::init(Scene* scene, Renderer* renderer)
{
	// Nothing needed anymore with the AssetRessource system
}

void Model::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	// Validate the pipeline type
	if (this->pipelineType.empty()) {
		throw std::runtime_error("failed to render model: pipeline type is not set!");
	}

	// Validate the mesh resource
	if (!m_meshResource) {
		throw std::runtime_error("failed to render model: mesh resource is null!");
	}

	// Get the model matrix and the active camera
	auto modelMatrix = this->getModelMatrix();
	auto currentCamera = renderer->getActiveCamera();

	// Bind the pipeline to render with
	renderer->bindPipeline(commandBuffer, this->pipelineType);

	// Bind the scene descriptor sets e.g. lights
	scene->bindSceneDescriptorSets(renderer, commandBuffer, currentFrame, this->pipelineType);

	// Bind the model matrix push constant
	renderer->bindPushConstants(commandBuffer, renderer->getCurrentPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UboModel), &modelMatrix);
	
	// Bind the model related descriptor sets
	VkDescriptorSet descriptorSet = renderer->getCameraDescriptorSet(currentCamera, currentFrame);
	renderer->bindDescriptorSet(descriptorSet, 0, currentFrame);
	
	// Render all meshes
	for (const auto& mesh : m_meshResource->meshes) {
		auto material = mesh->material.get();

		// Bind the material related descriptor sets
		material->bindMaterial(renderer, commandBuffer, 1, currentFrame);

		// Draw the mesh
		renderer->drawBuffers(mesh->vertexBufferIndex, mesh->indexBufferIndex, commandBuffer);
	}
}

void Model::destroy(Scene* scene, Renderer* renderer)
{
	// Nothing needed anymore with the AssetRessource system
}