#pragma once
#include "AssetRessource.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Mesh.h"
#include <vector>
class StaticMeshesRsc :
    public AssetRessource
{

public:
	std::vector<std::unique_ptr<Mesh>> meshes;

	StaticMeshesRsc() = default;
	~StaticMeshesRsc() = default;

	void loadFromFile(const std::string& path) override;
	void init(Renderer* renderer) override;
	void dispose(Renderer* renderer) override;
};

