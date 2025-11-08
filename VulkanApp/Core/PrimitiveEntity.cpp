#include "PrimitiveEntity.h"

PrimitiveEntity::PrimitiveEntity(const std::string& name, PrimitiveType primitiveType) : Entity(name)
{
	m_primitiveType = primitiveType;
}

PrimitiveEntity::PrimitiveEntity(const std::string& name, PrimitiveType primitiveTypem, const glm::vec4& color) : Entity(name)
{
	m_primitiveType = primitiveTypem;
	m_color = std::move(color);
}

void PrimitiveEntity::init(Scene* scene, Renderer* renderer)
{

}

void PrimitiveEntity::destroy(Scene* scene, Renderer* renderer)
{

}

void PrimitiveEntity::createAABB()
{
	switch (m_primitiveType) {
	case PrimitiveType::PRIMITIVE_TYPE_CUBE:
		setAABB(AABB(glm::vec3(-0.5f), glm::vec3(0.5f)));
		break;
	case PrimitiveType::PRIMITIVE_TYPE_SPHERE:
		setAABB(AABB(glm::vec3(-0.5f), glm::vec3(0.5f)));
		break;
	case PrimitiveType::PRIMITIVE_TYPE_QUAD:
		setAABB(AABB(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, 0.5f)));
		break;
	default:
		setAABB(AABB(glm::vec3(0.0f), glm::vec3(0.0f)));
		break;
	}
}

void PrimitiveEntity::render(Scene* scene, Renderer* renderer, VkCommandBuffer commandBuffer, int32_t currentFrame)
{
	if (this->hasState(EntityState::ENTITY_STATE_VISIBLE)) {
		Entity::render(scene, renderer, commandBuffer, currentFrame);
		renderer->drawPrimitive(m_primitiveType, this->transform.getMatrix(), glm::vec4(1), commandBuffer, currentFrame);
	}
}