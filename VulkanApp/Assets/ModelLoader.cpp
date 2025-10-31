#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/UnlitMaterial.h"
#include <iostream>

std::vector<std::unique_ptr<Mesh>> ModelLoader::loadModelFromFile(const std::string& file)
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
		UnlitMaterial* material = new UnlitMaterial();

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
			aiString texturePath;
			if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
				std::string fullPath = std::string(texturePath.C_Str());
				std::string directory = file.substr(0, file.find_last_of('/'));
				fullPath = directory + "/" + fullPath;

				auto texture = std::make_unique<ImageTexture>(fullPath);
				material->setAlbedoTexture(std::move(texture));
			}
			else {
				throw std::runtime_error("Model material does not have a diffuse texture!");
			}

			if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS) {
				std::string fullPath = std::string(texturePath.C_Str());
				std::string directory = file.substr(0, file.find_last_of('/'));
				fullPath = directory + "/" + fullPath;
				auto texture = std::make_unique<ImageTexture>(fullPath);
				material->normalTexture = std::move(texture);
			}
			else {
				// It's okay if there's no normal texture, just log a warning
				std::cout << "Warning: Model material has no normal texture!" << std::endl;
			}
		}

		mesh->material = std::unique_ptr<Material>(material);
		meshes.push_back(std::unique_ptr<Mesh>(mesh));

		return meshes;
	}

}
