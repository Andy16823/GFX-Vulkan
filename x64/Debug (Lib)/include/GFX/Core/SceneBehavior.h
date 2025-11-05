#pragma once
#include <string>
#include "../Graphics/Renderer.h"

class Scene;

class SceneBehavior
{
public:
	virtual ~SceneBehavior() = default;
	virtual void init(Scene* scene, Renderer* renderer) = 0;
	virtual void update(Scene* scene, float dt) = 0;
	virtual void destroy(Scene* scene, Renderer* renderer) = 0;
	virtual void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) = 0;
	virtual void beforeSwapchainRecreation(Scene* scene, Renderer* renderer) = 0;
	virtual void afterSwapchainRecreation(Scene* scene, Renderer* renderer, const glm::ivec2& size) = 0;
};

