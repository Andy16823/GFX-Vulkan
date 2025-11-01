#include "PBRMaterial.h"
#include "Renderer.h"

void PBRMaterial::init(Renderer* renderer)
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

	if (metRoughTexture) {
		metRoughTexture->bufferIndex = renderer->createImageBuffer(metRoughTexture.get());
		metRoughTexture->freeImageData(); // Free image data after uploading to GPU
	}
	else
	{
		// It's okay if there's no metRough texture, just log a warning
		std::cout << "Warning: UnlitMaterial has no metRough texture!" << std::endl;
	}

	if (aoTexture) {
		aoTexture->bufferIndex = renderer->createImageBuffer(aoTexture.get());
		aoTexture->freeImageData(); // Free image data after uploading to GPU
	}
	else {
		// It's okay if there's no ao texture, just log a warning
		std::cout << "Warning: UnlitMaterial has no ao texture!" << std::endl;
	}
}

void PBRMaterial::dispose(Renderer* renderer)
{
	// No need to do anything here since the renderer will dispose the image buffer itself
}

std::vector<int> PBRMaterial::getTextureIndices()
{
	return { 
		albedoTexture->bufferIndex, 
		normalTexture->bufferIndex, 
		metRoughTexture->bufferIndex, 
		aoTexture->bufferIndex 
	};
}
