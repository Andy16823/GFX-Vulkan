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
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	int textureIndex = -1;

	void setModelMatrix(glm::mat4 model);
	const Model& getModel() const;

	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);
	~Mesh();
	void dispose();
};

