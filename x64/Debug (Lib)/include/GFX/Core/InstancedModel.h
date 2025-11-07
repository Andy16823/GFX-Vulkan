#pragma once
#include "Scene.h"
#include "Instancer.h"
#include <string>
#include "../Graphics/Mesh.h"
#include "../Assets/StaticMeshesRsc.h"

/// <summary>
/// Instanced Model Entity
/// </summary>
class InstancedModel : public Instancer
{
private:
	/// <summary>
	/// The Pointer to the mesh resource containing the meshes for this model
	/// </summary>
	StaticMeshesRsc* m_meshResource;

public:
	InstancedModel(const std::string& name, StaticMeshesRsc* ressource, int instances);
	InstancedModel(const std::string& name, StaticMeshesRsc* ressource, const std::vector<InstanceData>& startValues, int instances);
	~InstancedModel() = default;

	/// <summary>
	/// Render the instanced model
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame) override;

	void update(Scene* scene, float dt) override;

	/// <summary>
	/// Create the axis-aligned bounding box for the instanced model
	/// </summary>
	void createAABB() override;
};

