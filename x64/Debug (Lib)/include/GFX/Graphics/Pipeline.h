#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "../Utils.h"

// TODO: Reduce create parameters and use more public members
class Pipeline
{
private:
	ShaderSourceCollection m_shaderSources;
	std::vector<VertexAttributeInfo> m_vertexAttributeInofs;
	VertexBindingInfo m_vertexBindingInfo;
	VkPipelineLayout m_pipelineLayout;


public:
	VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;
	VkBool32 depthTestEnable = VK_TRUE;
	VkBool32 depthWriteEnable = VK_TRUE;
	VkPipeline pipeline;

	VkPipelineLayout getPipelineLayout() const {
		return m_pipelineLayout;
	}

	Pipeline(ShaderSourceCollection shaderSources, VertexBindingInfo bindingInfo);
	void addVertexAttribute(const VertexAttributeInfo& attributeInfo) {
		m_vertexAttributeInofs.push_back(attributeInfo);
	}

	void createPipelineLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayouts, uint32_t descriptorSetLayoutCount, VkPushConstantRange* pushConstantRanges = nullptr, uint32_t pushConstantRangeCount = 0);
	void createPipeline(VkDevice device, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor);
	static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

	void destroy(VkDevice device) {
		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
	}
};

