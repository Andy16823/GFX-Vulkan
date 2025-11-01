#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Entity.h"
#include "../Utils.h"
#include "../Graphics/Mesh.h"
#include "../Assets/StaticMeshesRsc.h"

class Model :
    public Entity
{
private:
	StaticMeshesRsc* m_meshResource;

public:
    Model(std::string name, StaticMeshesRsc* ressource);
	~Model() = default;

	void update(float dt) override;;
	void init(Renderer* renderer);
	void render(Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame);
	void destroy(Renderer* renderer);
	Mesh* getMesh(int index);
};

