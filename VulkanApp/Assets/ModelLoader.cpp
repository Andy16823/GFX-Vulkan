#include "ModelLoader.h"
#include "../Graphics/PBRMaterial.h"
#include <iostream>
#include "../Utils.h"

std::unique_ptr<PBRMaterial> ModelLoader::loadPBRMaterial(const aiMaterial* aiMaterial, const std::string& file)
{
	auto material = std::make_unique<PBRMaterial>();
	
	aiColor4D color;
	if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
		material->properties.albedoColor = glm::vec4(color.r, color.g, color.b, color.a);
		std::cout << "[MODEL LOADER] Loaded albedo color: ("
			<< color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
	}
	else {
		material->properties.albedoColor = glm::vec4(1.0f);
		std::cout << "[MODEL LOADER] Warning: Model material has no albedo color! Using default white color instead." << std::endl;
	}

	// Load Albedo Texture
	aiString texturePath;
	if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
		std::string fullPath = std::string(texturePath.C_Str());
		std::string directory = file.substr(0, file.find_last_of('/'));
		fullPath = directory + "/" + fullPath;
		auto texture = std::make_unique<ImageTexture>(fullPath);
		material->setAlbedoTexture(std::move(texture));
		std::cout << "[MODEL LOADER] Loaded albedo texture." << std::endl;
	}
	else {
		std::cout << "[MODEL LOADER] Warning: Model material has no albedo texture! Using blank white texture instead." << std::endl;
		auto blankAlbedoTextureData = createTextureData(1, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		auto texture = std::make_unique<ImageTexture>(1, 1, blankAlbedoTextureData);
		material->setAlbedoTexture(std::move(texture));
	}

	// Load Normal Texture
	if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS) {
		std::string fullPath = std::string(texturePath.C_Str());
		std::string directory = file.substr(0, file.find_last_of('/'));
		fullPath = directory + "/" + fullPath;
		auto texture = std::make_unique<ImageTexture>(fullPath);
		material->setNormalTexture(std::move(texture));
		std::cout << "[MODEL LOADER] Loaded normal texture." << std::endl;
	}
	else {
		std::cout << "[MODEL LOADER] Warning: Model material has no normal texture! Using default normal map instead." << std::endl;
		auto defaultNormalTextureData = createTextureData(1, 1, glm::vec4(0.5f, 0.5f, 1.0f, 1.0f));
		auto texture = std::make_unique<ImageTexture>(1, 1, defaultNormalTextureData);
		material->setNormalTexture(std::move(texture));
	}

	// Load Metallic-Roughness Texture
	if (aiMaterial->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &texturePath)) {
		std::string fullPath = std::string(texturePath.C_Str());
		std::string directory = file.substr(0, file.find_last_of('/'));
		fullPath = directory + "/" + fullPath;
		auto texture = std::make_unique<ImageTexture>(fullPath);
		material->setMetRoughTexture(std::move(texture));
		std::cout << "[MODEL LOADER] Loaded metallic-roughness texture." << std::endl;
	}
	else {
		std::cout << "[MODEL LOADER] Warning: Model material has no metallic-roughness texture! Using default texture instead." << std::endl;
		auto defaultMetRoughTextureData = createTextureData(1, 1, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f)); // Green = Roughness 0.5, Blue = Metallic 0.5
		auto texture = std::make_unique<ImageTexture>(1, 1, defaultMetRoughTextureData);
		material->setMetRoughTexture(std::move(texture));
	}

	// Load Ambient Occlusion Texture
	if (aiMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath)) {
		std::string fullPath = std::string(texturePath.C_Str());
		std::string directory = file.substr(0, file.find_last_of('/'));
		fullPath = directory + "/" + fullPath;
		auto texture = std::make_unique<ImageTexture>(fullPath);
		material->setAOTexture(std::move(texture));
		std::cout << "[MODEL LOADER] Loaded ambient occlusion texture." << std::endl;
	}
	else {
		std::cout << "[MODEL LOADER] Warning: Model material has no ambient occlusion texture! Using default white texture instead." << std::endl;
		auto defaultAOTextureData = createTextureData(1, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		auto texture = std::make_unique<ImageTexture>(1, 1, defaultAOTextureData);
		material->setAOTexture(std::move(texture));
	}
	return material;
}

std::unique_ptr<UnlitMaterial> ModelLoader::loadUnlitMaterial(const aiMaterial* aiMaterial, const std::string& file)
{
	// Load Albedo Texture
	aiString texturePath;
	auto material = std::make_unique<UnlitMaterial>();
	if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
		std::string fullPath = std::string(texturePath.C_Str());
		std::string directory = file.substr(0, file.find_last_of('/'));
		fullPath = directory + "/" + fullPath;
		auto texture = std::make_unique<ImageTexture>(fullPath);
		material->albedoTexture = std::move(texture);
		std::cout << "[MODEL LOADER] Loaded albedo texture." << std::endl;
	}
	else {
		std::cout << "[MODEL LOADER] Warning: Model material has no albedo texture! Using blank white texture instead." << std::endl;
		auto blankAlbedoTextureData = createTextureData(1, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		auto texture = std::make_unique<ImageTexture>(1, 1, blankAlbedoTextureData);
		material->albedoTexture = std::move(texture);
	}
	return material;
}

std::vector<std::unique_ptr<Mesh>> ModelLoader::loadModelFromFile(const std::string& file, MaterialLoadingMode loadingMode)
{
	std::vector<std::unique_ptr<Mesh>> meshes;
	
	// Create an assimp importer and read the file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
	}

	// Process each mesh in the scene
	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		auto aiMesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh();

		// Process vertices
		std::vector<Vertex> vertices;
		for (size_t j = 0; j < aiMesh->mNumVertices; j++) {
			Vertex vertex = {};
			auto aiVertex = aiMesh->mVertices[j];
			auto aiNormal = aiMesh->mNormals[j];
			vertex.pos = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);

			if (aiMesh->HasVertexColors(0)) {
				auto aiColor = aiMesh->mColors[0][j];
				vertex.color = glm::vec3(aiColor.r, aiColor.g, aiColor.b);
			}
			else {
				vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
			}
			auto aiTexCoord = aiMesh->mTextureCoords[0][j];
			vertex.texCoord = glm::vec2(aiTexCoord.x, aiTexCoord.y);
			vertex.normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
			vertices.push_back(vertex);
		}
		mesh->setVertices(vertices);

		// Process indices
		std::vector<uint32_t> indices;
		for (size_t j = 0; j < aiMesh->mNumFaces; j++) {
			auto aiFace = aiMesh->mFaces[j];
			for (size_t k = 0; k < aiFace.mNumIndices; k++) {
				indices.push_back(aiFace.mIndices[k]);
			}
		}
		mesh->setIndices(indices);

		// Process material
		if (aiMesh->mMaterialIndex >= 0) {
			auto aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

			switch (loadingMode)
			{
			case MaterialLoadingMode::LOAD_MATERIALS_PBR:
				mesh->material = loadPBRMaterial(aiMaterial, file);
				break;
			case MaterialLoadingMode::LOAD_MATERIALS_UNLIT:
				mesh->material = loadUnlitMaterial(aiMaterial, file);
				break;
			default:
				throw std::runtime_error("Unsupported material loading mode!");
				break;
			}
		}
		else {
			throw std::runtime_error("Mesh has no material assigned!");
		}
		meshes.push_back(std::unique_ptr<Mesh>(mesh));
	}
	return meshes;
}
