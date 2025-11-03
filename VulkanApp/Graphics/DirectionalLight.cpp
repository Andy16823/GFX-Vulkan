#include "DirectionalLight.h"

void DirectionalLight::init(Renderer* renderer)
{
	size_t numFrames = renderer->numSwapChainImages();
	m_lightRessources.uniformBufferIndices.resize(numFrames);

	for (size_t i = 0; i < numFrames; i++) {
		m_lightRessources.uniformBufferIndices[i] = renderer->createUniformBuffer(sizeof(DirectionalLightData));
	}
}

void DirectionalLight::bind(Renderer* renderer, VkCommandBuffer commandBuffer, int frame, const std::string& pipeline)
{
	// Eearly out if no binding infos
	if (m_bindingInfos.empty()) {
		return;
	}

	// Get buffer index for the current frame
	int bufferIndex = m_lightRessources.uniformBufferIndices[frame];

	// Bind descriptor set
	UniformBuffer* buffer = renderer->getUniformBuffer(bufferIndex);
	VkDescriptorSet descriptorSet = renderer->getUniformBufferDescriptorSet(buffer->descriptorIndex);

	// Bind descriptor sets for all binding infos
	for (const auto& bindingInfo : m_bindingInfos)
	{
		if (bindingInfo.pipelineName != pipeline) {
			continue;
		}
		renderer->bindDescriptorSet(descriptorSet, bindingInfo.binding, frame);
		return;
	}
}

void DirectionalLight::dispose(Renderer* renderer)
{
	
}

void DirectionalLight::updateBuffers(Renderer* renderer, VkCommandBuffer commandBuffer, int frame)
{
	// Generate light data
	DirectionalLightData lightData = generateLightData();

	// Update uniform buffer
	int bufferIndex = m_lightRessources.uniformBufferIndices[frame];
	renderer->updateUniformBuffer(bufferIndex, &lightData, sizeof(lightData), 0);
}
