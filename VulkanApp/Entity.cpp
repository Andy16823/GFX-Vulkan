#include "Entity.h"

void Entity::setPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 Entity::getPosition()
{
	return m_position;
}

void Entity::setScale(glm::vec3 scale)
{
	m_scale = scale;
}

glm::vec3 Entity::getScale()
{
	return m_scale;
}

void Entity::setRotation(glm::quat rotation)
{
	m_rotation = rotation;
}

glm::quat Entity::getRotation()
{
	return m_rotation;
}

void Entity::rotate(float x, float y, float z)
{
	m_rotation = glm::rotate(m_rotation, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_rotation = glm::rotate(m_rotation, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_rotation = glm::rotate(m_rotation, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
}

Model Entity::getModelMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
	glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;


	return { modelMatrix };
}

void Entity::update(float dt)
{
	for (auto& component : m_components) {
		component->update(dt);
	}
}