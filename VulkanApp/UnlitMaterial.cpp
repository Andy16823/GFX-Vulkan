#include "UnlitMaterial.h"
#include "Renderer.h"

void UnlitMaterial::init(Renderer* renderer)
{
	if (albedoTexture) {
		albedoTexture->bufferIndex = renderer->createImageBuffer(albedoTexture.get());
		albedoTexture->freeImageData(); // Free image data after uploading to GPU
	}
	else {
		throw std::runtime_error("UnlitMaterial requires an albedo texture!");
	}
}

void UnlitMaterial::dispose(Renderer* renderer)
{
	// No need to do anything here since the renderer will dispose the image buffer itself
}

std::vector<int> UnlitMaterial::getTextureIndices()
{
	return { albedoTexture->bufferIndex };
}
