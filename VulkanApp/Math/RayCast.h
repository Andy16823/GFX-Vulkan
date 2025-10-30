#pragma once
#include <glm/glm.hpp>
#include "../Graphics/Camera.h"
class RayCast
{
public:
	static glm::vec3 castRayFromMouse(glm::vec2 mousePos, glm::vec2 viewport, Camera* camera, float groundHeight = 0.0f, bool invertZ = true);
};

