#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/PBRMaterial.h"
#include "../Graphics/UnlitMaterial.h"

enum class MaterialLoadingMode {
	LOAD_MATERIALS_PBR,
	LOAD_MATERIALS_UNLIT
};

class ModelLoader
{
private:
	static std::unique_ptr<PBRMaterial> loadPBRMaterial(const aiMaterial* aiMaterial, const std::string& file);
	static std::unique_ptr<UnlitMaterial> loadUnlitMaterial(const aiMaterial* aiMaterial, const std::string& file);
public: 
	static std::vector<std::unique_ptr<Mesh>> loadModelFromFile(const std::string& file, MaterialLoadingMode loadingMode);
};

