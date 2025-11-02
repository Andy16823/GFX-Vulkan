#pragma once
#include "Material.h"
#include <glm/glm.hpp>

struct PBRMaterialProperties
{
	glm::vec4 albedoColor = glm::vec4(1.0f);
};

/// <summary>
/// Physically Based Rendering Material
/// </summary>
class PBRMaterial :	public Material
{
private:
	int m_uniformBufferIndex = -1;

public:
	/// <summary>
	/// Albedo texture
	/// </summary>
	std::unique_ptr<ImageTexture> albedoTexture;

	/// <summary>
	/// Normal texture
	/// </summary>
	std::unique_ptr<ImageTexture> normalTexture;

	/// <summary>
	/// Metallic-Roughness texture
	/// </summary>
	std::unique_ptr<ImageTexture> metRoughTexture;

	/// <summary>
	/// Ambient Occlusion texture
	/// </summary>
	std::unique_ptr<ImageTexture> aoTexture;

	/// <summary>
	/// PBR Material Properties for shader use
	/// </summary>
	PBRMaterialProperties properties;


	PBRMaterial() = default;
	~PBRMaterial() = default;

	/// <summary>
	/// Initialize the material (create and upload textures to GPU)
	/// </summary>
	/// <param name="renderer"></param>
	void init(Renderer* renderer) override;

	/// <summary>
	/// Dispose the material (free GPU resources)
	/// </summary>
	/// <param name="renderer"></param>
	void dispose(Renderer* renderer) override;

	/// <summary>
	/// Bind the material (bind descriptor sets)
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="firstSet"></param>
	/// <param name="frame"></param>
	void bindMaterial(Renderer* renderer, VkCommandBuffer commandBuffer, int firstSet, int frame) override;

	/// <summary>
	/// Set the albedo texture
	/// </summary>
	/// <param name="texture"></param>
	void setAlbedoTexture(std::unique_ptr<ImageTexture> texture) { albedoTexture = std::move(texture); }

	/// <summary>
	/// Set the normal texture
	/// </summary>
	/// <param name="texture"></param>
	void setNormalTexture(std::unique_ptr<ImageTexture> texture) { normalTexture = std::move(texture); }

	/// <summary>
	/// Set the metallic-roughness texture
	/// </summary>
	/// <param name="texture"></param>
	void setMetRoughTexture(std::unique_ptr<ImageTexture> texture) { metRoughTexture = std::move(texture); }

	/// <summary>
	/// Set the ambient occlusion texture
	/// </summary>
	/// <param name="texture"></param>
	void setAOTexture(std::unique_ptr<ImageTexture> texture) { aoTexture = std::move(texture); }
};

