#pragma once
#include "Material.h"
#include "Renderer.h"

/// <summary>
/// Simple unlit material
/// </summary>
class UnlitMaterial : public Material
{
public:
	/// <summary>
	/// The albedo texture
	/// </summary>
	std::unique_ptr<ImageTexture> albedoTexture;

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
};