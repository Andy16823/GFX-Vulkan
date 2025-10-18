#include "PipelineManager.h"

Pipeline* PipelineManager::createPipeline(const std::string& name, ShaderSourceCollection shaderSources, VertexBindingInfo bindingInfo)
{
	std::unique_ptr<Pipeline> pipeline = std::make_unique<Pipeline>(shaderSources, bindingInfo);
	m_pipelines[name] = std::move(pipeline);
	return m_pipelines[name].get();
}

Pipeline* PipelineManager::getPipeline(const std::string& name)
{
	auto it = m_pipelines.find(name);
	return (it != m_pipelines.end()) ? it->second.get() : nullptr;
}

void PipelineManager::destroyAllPipelines(VkDevice device)
{
	for (auto& pair : m_pipelines) {
		pair.second->destroy(device);
	}
	m_pipelines.clear();
}
