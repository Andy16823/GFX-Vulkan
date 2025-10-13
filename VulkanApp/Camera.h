#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Utils.h"

class Camera
{
public:
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual UboViewProjection getViewProjection() = 0;
};

