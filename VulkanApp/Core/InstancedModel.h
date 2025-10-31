#pragma once
#include "Entity.h"
#include <string>
#include "../Graphics/Mesh.h"


/// <summary>
/// Instance data structure
/// Aligned to 16 bytes for Vulkan
/// Note: No padding needed as glm::mat4 is already 16-byte aligned
/// </summary>
struct InstanceData {
	glm::mat4 model;
	glm::vec4 extras;
};

/// <summary>
/// Instanced Model Entity
/// </summary>
class InstancedModel : public Entity
{
private:
	/// <summary>
	/// The meshes in the model
	/// </summary>
	std::vector<std::unique_ptr<Mesh>> m_meshes;

	/// <summary>
	/// Index of the storage buffer for instance data
	/// </summary>
	int m_storageBufferIndex = -1;

public:
	InstancedModel(const std::string name, const std::string file, const int instances);
	~InstancedModel() = default;

	/// <summary>
	/// The number of instances within the model
	/// </summary>
	int instanceCount = 0;

	/// <summary>
	/// Initialize the instanced model
	/// </summary>
	/// <param name="renderer"></param>
	void init(Renderer* renderer) override;

	/// <summary>
	/// Render the instanced model
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;

	/// <summary>
	/// Destroy the instanced model
	/// </summary>
	/// <param name="renderer"></param>
	void destroy(Renderer* renderer) override;

	/// <summary>
	/// Update the instance data for a specific instance
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="data"></param>
	/// <param name="instanceIndex"></param>
	void updateInstance(Renderer* renderer, const InstanceData& data, int instanceIndex);

	/// <summary>
	/// Update a range of instance data
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="instanceDataArray"></param>
	/// <param name="offset"></param>
	void updateInstanceRange(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray, int offset);

	/// <summary>
	/// Update all instance data at once
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="instanceDataArray"></param>
	void updateAllInstances(Renderer* renderer, const std::vector<InstanceData>& instanceDataArray);
};

