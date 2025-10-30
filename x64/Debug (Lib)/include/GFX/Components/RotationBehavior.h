#pragma once
#include "../Core/Behavior.h"

class RotationBehavior : public Behavior
{
public:
	void init(Renderer* renderer) override {}
	void update(float dt) override;
	void destroy(Renderer* renderer) override {}
	void render(Renderer* renderer) override {}
	std::string getIdentifier() override { return "RotationBehavior"; }
};

