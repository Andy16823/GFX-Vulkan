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

void Model::update(float dt)
{
	Entity::update(dt);
}


void Model::init(Renderer* renderer)
{
	// Nothing needed anymore with the AssetRessource system
}

/// <summary>
/// Render all meshes in the model
/// </summary>
/// <param name="renderer"></param>
/// <param name="currentFrame"></param>
void Model::render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	if (this->pipelineType.empty()) {
		throw std::runtime_error("failed to render model: pipeline type is not set!");
	}

	// Ensure the mesh resource is valid
	if (!m_meshResource) {
		throw std::runtime_error("failed to render model: mesh resource is null!");
	}

	auto modelMatrix = this->getModelMatrix();
	auto currentCamera = renderer->getActiveCamera();
	VkDescriptorSet descriptorSet = renderer->getCameraDescriptorSet(currentCamera, currentFrame);
	renderer->bindPipeline(commandBuffer, this->pipelineType); 
	renderer->bindPushConstants(commandBuffer, renderer->getCurrentPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UboModel), &modelMatrix);
	renderer->bindDescriptorSet(descriptorSet, 0, currentFrame);
	
	for (const auto& mesh : m_meshResource->meshes) {
		auto material = mesh->material.get();
		material->bindMaterial(renderer, commandBuffer, 1, currentFrame); // Bind material descriptor sets starting from set 1
		renderer->drawBuffers(mesh->vertexBufferIndex, mesh->indexBufferIndex, commandBuffer);
	}
}

/// <summary>
/// Destroy all meshes in the model
/// </summary>
/// <param name="renderer"></param>
void Model::destroy(Renderer* renderer)
{
	// Nothing needed anymore with the AssetRessource system
}