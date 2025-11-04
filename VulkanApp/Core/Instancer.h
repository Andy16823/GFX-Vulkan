#pragma once
#include "Entity.h"
#include "InstanceHandle.h"

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
/// Base Instancer Entity
/// </summary>
class Instancer : public Entity
{
private:
	/// <summary>
	/// Index of the storage buffer for instance data
	/// </summary>
	int m_storageBufferIndex = -1;

	/// <summary>
	/// Starting values for instances
	/// </summary>
	std::vector<InstanceData> m_instanceStartValues;
public:
	/// <summary>
	/// Create an instancer with a specific number of instances
	/// </summary>
	/// <param name="instances"></param>
	Instancer(const std::string& name, int instances);

	/// <summary>
	/// Create an instancer with specific starting values
	/// </summary>
	/// <param name="startValues"></param>
	/// <param name="instances"></param>
	Instancer(const std::string& name, const std::vector<InstanceData>& startValues, int instances);

	/// <summary>
	/// The number of instances managed by this instancer
	/// </summary>
	int instanceCount = 0;

	/// <summary>
	/// Get the storage buffer index
	/// </summary>
	/// <returns></returns>
	int getStorageBufferIndex() const { return m_storageBufferIndex; }

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

	/// <summary>
	/// Initialize the instancer
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void init(Scene* scene, Renderer* renderer) override;

	/// <summary>
	/// Destroy the instancer
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void destroy(Scene* scene, Renderer* renderer) override {};

	/// <summary>
	/// Create an instance handle for a specific instance
	/// </summary>
	/// <param name="name"></param>
	/// <param name="instanceId"></param>
	/// <returns></returns>
	std::unique_ptr<InstanceHandle> createInstanceHandle(const std::string& name, int instanceId);
};

