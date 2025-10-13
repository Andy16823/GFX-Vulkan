#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Renderer.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

class Entity
{
private:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

public:
	Entity() = default;
	virtual ~Entity() = default;
	
	// Transforms
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setScale(glm::vec3 scale);
	glm::vec3 getScale();
	void setRotation(glm::quat rotation);
	glm::quat getRotation();
	void rotate(float x, float y, float z);
	Model getModelMatrix();

	virtual void update(float dt) = 0;
	virtual void init(Renderer* renderer) = 0;
	virtual void render(Renderer* renderer, int32_t currentFrame) = 0;
	virtual void destroy(Renderer* renderer) = 0;
};

