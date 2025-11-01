#include "StaticMeshesRsc.h"
#include "ModelLoader.h"

void StaticMeshesRsc::loadFromFile(const std::string& path)
{
	this->meshes = ModelLoader::loadModelFromFile(path);
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
