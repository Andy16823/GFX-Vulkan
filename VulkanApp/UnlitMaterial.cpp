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

	if (normalTexture) {
		normalTexture->bufferIndex = renderer->createImageBuffer(normalTexture.get());
		normalTexture->freeImageData(); // Free image data after uploading to GPU
	}
	else {
		// It's okay if there's no normal texture, just log a warning
		std::cout << "Warning: UnlitMaterial has no normal texture!" << std::endl;
	}
}

void UnlitMaterial::dispose(Renderer* renderer)
{
	// No need to do anything here since the renderer will dispose the image buffer itself
}

std::vector<int> UnlitMaterial::getTextureIndices()
{
	return { albedoTexture->bufferIndex, normalTexture->bufferIndex };
}
