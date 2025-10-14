#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include "Material.h"

/// <summary>
/// Representation of an mesh witch contains vertices, indices and a material
/// </summary>
class Mesh
{
private:

public:
	std::unique_ptr<Material> material;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	int vertexBufferIndex = -1;
	int indexBufferIndex = -1;

	Mesh() = default;
	~Mesh() = default;

	void init(Renderer* renderer);
	void setVertices(std::vector<Vertex> vertices) { this->vertices = vertices; }
	void setIndices(std::vector<uint32_t> indices) { this->indices = indices; }

	void dispose(Renderer* renderer);
};

