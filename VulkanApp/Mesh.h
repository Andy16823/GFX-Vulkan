#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

struct Model {
	glm::mat4 model;
};

class Mesh
{
private:
	Model m_uboModel;
public:
	int vertexBufferIndex = -1;
	int indexBufferIndex = -1;
	int textureIndex = -1;

	void setModelMatrix(glm::mat4 model);
	const Model& getModel() const;
	Mesh();
	~Mesh();

	void dispose();
};

