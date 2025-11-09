#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Utils.h"
#include "../Math/Transform.h"
#include "../Math/Frustum.h"

class Camera
{
public:
	Transform transform;
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual UboViewProjection getViewProjection() = 0;
	virtual void moveForward(float distance) = 0;
	virtual void moveUp(float distance) = 0;
	virtual void moveRight(float distance) = 0;
	virtual void turn(float pitch, float yaw, float roll) = 0;
	virtual glm::vec3 projectPosition(const glm::vec3& worldpos, const glm::vec2 viewport) = 0;
	virtual glm::vec3 unprojectPosition(const glm::vec3& screenpos, const glm::vec2 viewport) = 0;
	virtual Frustum getFrustum() const = 0;
};

