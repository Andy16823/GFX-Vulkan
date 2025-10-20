#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "../Utils.h"

/// <summary>
/// Binding info for vertex input
/// </summary>
struct VertexBindingInfo
{
	uint32_t binding;
	uint32_t stride;
	VkVertexInputRate inputRate;
};

/// <summary>
/// Vertex attribute info for vertex input
/// </summary>
struct VertexAttributeInfo
{
	uint32_t binding;
	uint32_t location;
	VkFormat format;
	uint32_t offset;
};

class Pipeline
{
private:
	ShaderSourceCollection m_shaderSources;
	std::vector<VertexAttributeInfo> m_vertexAttributeInofs;
	VertexBindingInfo m_vertexBindingInfo;


public:
	VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;
	VkBool32 depthTestEnable = VK_TRUE;
	VkBool32 depthWriteEnable = VK_TRUE;
	VkPipeline pipeline;

	Pipeline(ShaderSourceCollection shaderSources, VertexBindingInfo bindingInfo);
	void addVertexAttribute(const VertexAttributeInfo& attributeInfo) {
		m_vertexAttributeInofs.push_back(attributeInfo);
	}
	void createPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor);
	static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

	void destroy(VkDevice device) {
		vkDestroyPipeline(device, pipeline, nullptr);
	}
};

