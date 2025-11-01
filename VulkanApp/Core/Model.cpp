#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/PBRMaterial.h"
#include "../Assets/ModelLoader.h"


Model::Model(std::string name, StaticMeshesRsc* ressource) 
	: Entity(name)
{
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
	if (!m_meshResource) {
		throw std::runtime_error("failed to render model: mesh resource is null!");
	}

	// Render the model with the meshes from the resource
	auto currentCamera = renderer->getActiveCamera();
	VkDescriptorSet descriptorSet = renderer->getCameraDescriptorSet(currentCamera, currentFrame);	 // Get the descriptor set for the current camera
	renderer->bindPipeline(commandBuffer, ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D)); // Bind the 3D graphics pipeline

	for (const auto& mesh : m_meshResource->meshes) {
		std::vector<VkDescriptorSet> desriptorSets;
		desriptorSets.push_back(descriptorSet); // Add the per-frame UBO descriptor set first
		auto textureBufferIndices = mesh->material->getTextureIndices(); // Get the indices to the buffers from the material textures
		for (size_t i = 0; i < textureBufferIndices.size(); i++) {
			int textureBufferIndex = textureBufferIndices[i];
			VkDescriptorSet textureDescriptorSet = renderer->getSamplerDescriptorSetFromImageBuffer(textureBufferIndex);
			desriptorSets.push_back(textureDescriptorSet);
		}

		// Bind pipeline, descriptor sets and draw the mesh
		renderer->bindDescriptorSets(desriptorSets, currentFrame);
		renderer->drawMesh(mesh.get(), mesh->material.get(), this->getModelMatrix(), currentFrame);
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