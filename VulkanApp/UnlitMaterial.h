#pragma once
#include "Material.h"

/// <summary>
/// Unlit material class
/// The UnlitMaterial class is a simple material that only has an albedo texture
/// </summary>
class UnlitMaterial :
	public Material
{
private:

public:
	std::unique_ptr<ImageTexture> albedoTexture;

	UnlitMaterial() = default;
	~UnlitMaterial() = default;
	void init(Renderer* renderer);
	void dispose(Renderer* renderer);
	std::vector<int> getTextureIndices();

	void setAlbedoTexture(std::unique_ptr<ImageTexture> texture) { albedoTexture = std::move(texture); }
};

