#include "RotationBehavior.h"
#include "Entity.h"

void RotationBehavior::update(float dt)
{
	this->parent->rotate(0.0f, 0.0f, 0.01f);
}
