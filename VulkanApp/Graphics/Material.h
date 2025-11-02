#pragma once
#include <vector>
#include "ImageTexture.h"
#include <string>

/// <summary>
/// Forward declaration of Renderer class
/// </summary>
class Renderer;

/// <summary>
/// Base Material class to be inherited by specific material implementations
/// getTextureIndices should return the indices of texture buffers used by the material
/// in the correct order as expected by the shader
/// </summary>
class Material
{
public:
	Material() = default;
	~Material() = default;

	std::vector<std::unique_ptr<ImageTexture>> textures;
	virtual void init(Renderer* renderer) = 0;
	virtual void dispose(Renderer* renderer) = 0;
	virtual void bindMaterial(Renderer* renderer, VkCommandBuffer commandBuffer, int firstSet, int frame) = 0;
};

