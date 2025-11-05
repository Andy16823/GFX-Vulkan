#pragma once
#include <string>
#include "../Utils.h"
#include "../Graphics/Renderer.h"

class Scene;
class Entity;

class Behavior
{
public:
	Entity* parent = nullptr;
	virtual ~Behavior() = default;

	virtual void init(Scene* scene, Renderer* renderer) = 0;
	virtual void update(Scene* scene, float dt) = 0;
	virtual void destroy(Scene* scene, Renderer* renderer) = 0;
	virtual void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) = 0;
	virtual std::string getIdentifier() = 0;
};

