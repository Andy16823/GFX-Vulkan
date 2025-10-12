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

Mesh::Mesh()
{
	m_uboModel.model = glm::mat4(1.0f);
}

Mesh::~Mesh()
{
	//delete vertexBuffer;
	//delete indexBuffer;
}

void Mesh::dispose()
{
	//vertexBuffer->dispose();
	//indexBuffer->dispose();
}
