#include "RotationBehavior.h"
#include "../Core/Entity.h"

void RotationBehavior::update(Scene* scene, float dt)
{
	this->parent->rotate(0.0f, 0.0f, 0.01f);
}
