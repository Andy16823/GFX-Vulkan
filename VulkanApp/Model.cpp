#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(std::string file) : Entity()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
	}

	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		auto aiMesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh();

		// Process vertices
		std::vector<Vertex> vertices;
		for (size_t j = 0; j < aiMesh->mNumVertices; j++) {
			Vertex vertex = {};
			auto aiVertex = aiMesh->mVertices[j];
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
		m_meshes.push_back(std::unique_ptr<Mesh>(mesh));
	}
}

void Model::init(Renderer* renderer)
{
	for (auto& mesh : m_meshes) {
		auto verticexs = mesh.get()->m_vertices;
		auto indices = mesh.get()->m_indices;
		mesh->vertexBufferIndex = renderer->createVertexBuffer(&verticexs);
		mesh->indexBufferIndex = renderer->createIndexBuffer(&indices);
	}
}

void Model::render(Renderer* renderer, int32_t currentFrame)
{
	for (auto& mesh : m_meshes) {
		renderer->drawMesh(mesh.get(), 0, this->getModelMatrix(), currentFrame);
	}
}

void Model::destroy(Renderer* renderer)
{

}
