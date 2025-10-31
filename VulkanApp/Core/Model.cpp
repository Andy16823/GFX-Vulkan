#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/UnlitMaterial.h"
#include "../Assets/ModelLoader.h"

/// <summary>
/// Load a model from a file using Assimp
/// </summary>
/// <param name="file"></param>
Model::Model(std::string name, std::string file) : Entity(name)
{
	m_meshes = ModelLoader::loadModelFromFile(file);
}

void Model::update(float dt)
{
	Entity::update(dt);
}

/// <summary>
/// Initialize all meshes in the model
/// </summary>
/// <param name="renderer"></param>
void Model::init(Renderer* renderer)
{
	for (auto& mesh : m_meshes) {
		mesh.get()->init(renderer);
	}
}

/// <summary>
/// Render all meshes in the model
/// </summary>
/// <param name="renderer"></param>
/// <param name="currentFrame"></param>
void Model::render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	auto currentCamera = renderer->getActiveCamera();
	VkDescriptorSet descriptorSet = renderer->getCameraDescriptorSet(currentCamera, currentFrame);	 // Get the descriptor set for the current camera
	renderer->bindPipeline(commandBuffer, ToString(PipelineType::PIPELINE_TYPE_GRAPHICS_3D)); // Bind the 3D graphics pipeline

	for (auto& mesh : m_meshes) {
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
	for (auto& mesh : m_meshes) {
		mesh.get()->dispose(renderer);
	}
}

Mesh* Model::getMesh(int index)
{
	if (index >= 0 && index < m_meshes.size()) {
		return m_meshes[index].get();
	}
	throw std::runtime_error("failed to get mesh: invalid mesh index!");
}
