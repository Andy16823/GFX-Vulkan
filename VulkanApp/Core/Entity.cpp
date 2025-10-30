#include "Entity.h"
#include "../Utils.h"

Entity::Entity(std::string name)
{
	this->name = name;
	this->UniqueId = generateUUID();
}

void Entity::setPosition(glm::vec3 position)
{
	transform.position = position;
}

glm::vec3 Entity::getPosition()
{
	return transform.position;
}

void Entity::setScale(glm::vec3 scale)
{
	transform.scale = scale;
}

glm::vec3 Entity::getScale()
{
	return transform.scale;
}

void Entity::setRotation(glm::quat rotation)
{
	transform.rotation = rotation;
}

glm::quat Entity::getRotation()
{
	return transform.rotation;
}

void Entity::rotate(float x, float y, float z)
{
	transform.rotate(x, y, z);
}

UboModel Entity::getModelMatrix()
{
	auto matrix = transform.getMatrix();
	return { matrix };
}

void Entity::update(float dt)
{
	for (auto& component : m_behaviors) {
		component->update(dt);
	}
}