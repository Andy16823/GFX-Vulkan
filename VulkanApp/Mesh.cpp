#include "Mesh.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

/// <summary>
/// Initialize the mesh by creating vertex and index buffers in the renderer
/// and dispose the local vertex and index data to save memory
/// </summary>
/// <param name="renderer"></param>
void Mesh::init(Renderer* renderer)
{
	this->material->init(renderer);
	this->vertexBufferIndex = renderer->createVertexBuffer(&this->vertices);
	this->indexBufferIndex = renderer->createIndexBuffer(&this->indices);
	this->vertices.clear();
	this->indices.clear();
}

/// <summary>
/// Dispose the mesh and free resources
/// No need to dispose vertex and index buffers since the renderer will handle that
/// </summary>
/// <param name="renderer"></param>
void Mesh::dispose(Renderer* renderer)
{
	this->material->dispose(renderer);
}
