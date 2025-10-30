#pragma once
#include <string>
#include "../Utils.h"
#include "../Graphics/Renderer.h"
class Entity;

class Behavior
{
public:
	Entity* parent = nullptr;
	virtual ~Behavior() = default;

	virtual void init(Renderer* renderer) = 0;
	virtual void update(float dt) = 0;
	virtual void destroy(Renderer* renderer) = 0;
	virtual void render(Renderer* renderer) = 0;
	virtual std::string getIdentifier() = 0;
};

