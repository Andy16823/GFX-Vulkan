#pragma once
#include "../Core/Behavior.h"
#include "../Core/Scene.h"

class RotationBehavior : public Behavior
{
public:
	void init(Scene* scene, Renderer* renderer) override {}
	void update(Scene* scene, float dt) override;
	void destroy(Scene* scene, Renderer* renderer) override {}
	void render(Scene* scene, Renderer* renderer, VkCommandBuffer commandbuffer, int frame) override {}
	std::string getIdentifier() override { return "RotationBehavior"; }
};

