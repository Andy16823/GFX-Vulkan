#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Entity.h"
#include "../Utils.h"
#include "../Graphics/Mesh.h"
#include "../Assets/StaticMeshesRsc.h"
#include "Scene.h"

class Model :
    public Entity
{
private:
	StaticMeshesRsc* m_meshResource;

public:
    Model(std::string name, StaticMeshesRsc* ressource);
	~Model() = default;

	void update(Scene* scene, float dt) override;;
	void init(Scene* scene, Renderer* renderer);
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame);
	void destroy(Scene* scene, Renderer* renderer);
	Mesh* getMesh(int index);
};

