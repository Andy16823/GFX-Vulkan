#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include "Pipeline.h"

/// <summary>
/// Class to manage multiple pipelines
/// </summary>
class PipelineManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<Pipeline>> m_pipelines;

public:
	PipelineManager() = default;
	~PipelineManager() = default;
	Pipeline* createPipeline(const std::string& name, ShaderSourceCollection shaderSources, VertexBindingInfo bindingInfo);
	Pipeline* getPipeline(const std::string& name);
	void destroyAllPipelines(VkDevice device);
};

