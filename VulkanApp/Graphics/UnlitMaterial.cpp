#include "UnlitMaterial.h"

void UnlitMaterial::init(Renderer* renderer)
{
	this->albedoTexture->bufferIndex = renderer->createImageBuffer(albedoTexture.get());
	this->albedoTexture->freeImageData();
}

void UnlitMaterial::dispose(Renderer* renderer)
{

}

void UnlitMaterial::bindMaterial(Renderer* renderer, VkCommandBuffer commandBuffer, int firstSet, int frame)
{
	VkDescriptorSet albedoSet = renderer->getSamplerDescriptorSetFromImageBuffer(albedoTexture->bufferIndex);
	renderer->bindDescriptorSet(albedoSet, firstSet, frame);
}
