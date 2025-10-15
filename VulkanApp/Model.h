#pragma once
#include <string>
#include <vector>
#include "Entity.h"
#include "Utils.h"
#include "Mesh.h"

class Model :
    public Entity
{
private:
	std::vector<std::unique_ptr<Mesh>> m_meshes;

public:
    Model(std::string name, std::string file);
	~Model() = default;

	void update(float dt) override {};
	void init(Renderer* renderer);
	void render(Renderer* renderer, int32_t currentFrame);
	void destroy(Renderer* renderer);
};

