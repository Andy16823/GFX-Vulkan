#pragma once
#include "Material.h"

/// <summary>
/// Unlit material class
/// The UnlitMaterial class is a simple material that only has an albedo texture
/// </summary>
class PBRMaterial :
	public Material
{
private:

public:
	std::unique_ptr<ImageTexture> albedoTexture;
	std::unique_ptr<ImageTexture> normalTexture;
	std::unique_ptr<ImageTexture> metRoughTexture;
	std::unique_ptr<ImageTexture> aoTexture;


	PBRMaterial() = default;
	~PBRMaterial() = default;
	void init(Renderer* renderer);
	void dispose(Renderer* renderer);
	std::vector<int> getTextureIndices();

	void setAlbedoTexture(std::unique_ptr<ImageTexture> texture) { albedoTexture = std::move(texture); }
	void setNormalTexture(std::unique_ptr<ImageTexture> texture) { normalTexture = std::move(texture); }
	void setMetRoughTexture(std::unique_ptr<ImageTexture> texture) { metRoughTexture = std::move(texture); }
	void setAOTexture(std::unique_ptr<ImageTexture> texture) { aoTexture = std::move(texture); }
};

