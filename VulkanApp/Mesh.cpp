#include "Mesh.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Mesh::setModelMatrix(glm::mat4 model)
{
	m_uboModel.model = model;
}

const Model& Mesh::getModel() const
{
	return m_uboModel;
}

Mesh::Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices)
{
	vertexBuffer = new VertexBuffer(newPhysicalDevice, newDevice, transferQueue, transferCommandPool, vertices);
	indexBuffer = new IndexBuffer(newPhysicalDevice, newDevice, transferQueue, transferCommandPool, indices);
	m_uboModel.model = glm::mat4(1.0f);
}

Mesh::~Mesh()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void Mesh::dispose()
{
	vertexBuffer->dispose();
	indexBuffer->dispose();
}
