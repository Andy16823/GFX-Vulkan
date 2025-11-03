#pragma once
#include "Light.h"

/// <summary>
/// The data structure for a directional light
/// Used in the shader
/// </summary>
struct DirectionalLightData
{
	glm::vec4 direction;
	glm::vec4 colorIntensity;
};

/// <summary>
/// Binding info for directional light
/// </summary>
struct DirectionalLightBindingInfo {
	const std::string pipelineName;		// The name of the pipeline
	const uint32_t binding;				// The binding index
};


/// <summary>
/// Directional light class
/// </summary>
class DirectionalLight : public Light
{
private:
	
	/// <summary>
	///	Resources for the directional light
	/// </summary>
	struct DirectionalLightRescources {
		std::vector<int> uniformBufferIndices;
	} m_lightRessources;

	/// <summary>
	/// Binding infos for the directional light
	/// </summary>
	std::vector<DirectionalLightBindingInfo> m_bindingInfos;

	/// <summary>
	/// Generate the light data
	/// </summary>
	/// <returns></returns>
	DirectionalLightData generateLightData()
	{
		DirectionalLightData data;
		data.direction = glm::vec4(glm::normalize(direction), 0.0f);
		data.colorIntensity = glm::vec4(color, intensity);
		return data;
	}
public:

	/// <summary>
	/// The direction of the light
	/// </summary>
	glm::vec3 direction = glm::vec3(-0.5f, -1.0f, -0.3f);

	/// <summary>
	/// The color of the light
	/// </summary>
	glm::vec3 color = glm::vec3(1.0f, 0.95f, 0.9f);

	/// <summary>
	/// The intensity of the light
	/// </summary>
	float intensity = 1;

	/// <summary>
	/// Initialize the light
	/// </summary>
	/// <param name="renderer"></param>
	void init(Renderer* renderer) override;

	/// <summary>
	/// Bind the light
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	/// <param name="firstSet"></param>
	void bind(Renderer* renderer, VkCommandBuffer commandBuffer, int frame, const std::string& pipeline) override;

	/// <summary>
	/// Dispose the light
	/// </summary>
	/// <param name="renderer"></param>
	void dispose(Renderer* renderer) override;

	/// <summary>
	/// Update the light buffers for the current frame
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="frame"></param>
	void updateBuffers(Renderer* renderer, VkCommandBuffer commandBuffer, int frame) override;

	/// <summary>
	/// Add binding info for the directional light
	/// </summary>
	/// <param name="bindingInfo"></param>
	void addBindingInfo(const DirectionalLightBindingInfo& bindingInfo) {
		m_bindingInfos.push_back(bindingInfo);
	}
};

