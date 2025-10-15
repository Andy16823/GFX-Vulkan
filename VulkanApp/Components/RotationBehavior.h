#pragma once
#include "../Core/Component.h"

class RotationBehavior : public Component
{
public:
	void init(Renderer* renderer) override {}
	void update(float dt) override;
	void destroy(Renderer* renderer) override {}
	void render(Renderer* renderer) override {}
	std::string getIdentifier() override { return "RotationBehavior"; }
};

