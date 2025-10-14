#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

class Mesh
{
private:

public:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	int vertexBufferIndex = -1;
	int indexBufferIndex = -1;
	Mesh();
	void setVertices(std::vector<Vertex> vertices) { m_vertices = vertices; }
	void setIndices(std::vector<uint32_t> indices) { m_indices = indices; }
	~Mesh();

	void dispose();
};

