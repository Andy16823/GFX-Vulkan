#include "StaticMeshesRsc.h"
#include "ModelLoader.h"

void StaticMeshesRsc::loadFromFile(const std::string& path, MaterialLoadingMode loadingMode)
{
	this->meshes = ModelLoader::loadModelFromFile(path, loadingMode);
}

void StaticMeshesRsc::init(Renderer* renderer)
{
	for (const auto& mesh : meshes) {
		mesh->init(renderer);
	}
}

void StaticMeshesRsc::dispose(Renderer* renderer)
{
	for (const auto& mesh : meshes) {
		mesh->dispose(renderer);
	}
}
