#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Entity.h"
#include "../Utils.h"
#include "../Graphics/Mesh.h"
#include "../Assets/StaticMeshesRsc.h"
#include "Scene.h"

/// <summary>
/// Single Model Entity
/// </summary>
class Model :
    public Entity
{
private:
	/// <summary>
	/// The Pointer to the mesh resource containing the meshes for this model
	/// </summary>
	StaticMeshesRsc* m_meshResource;

public:
    /// <summary>
	/// Create a model from a mesh resource
    /// </summary>
    /// <param name="name"></param>
    /// <param name="ressource"></param>
    Model(std::string name, StaticMeshesRsc* ressource);
	~Model() = default;

	/// <summary>
	/// Update the model
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="dt"></param>
	void update(Scene* scene, float dt) override;;

	/// <summary>
	/// Initialize the model
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void init(Scene* scene, Renderer* renderer);

	/// <summary>
	/// Render the model
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	/// <param name="commandBuffer"></param>
	/// <param name="currentFrame"></param>
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame);

	/// <summary>
	/// Destroy the model
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="renderer"></param>
	void destroy(Scene* scene, Renderer* renderer);

	/// <summary>
	/// Create the axis-aligned bounding box for the model
	/// </summary>
	void createAABB() override;

};

