#pragma once
#include "Entity.h"
#include "InstancedModel.h"

class InstanceHandle : public Entity
{
private:
	InstancedModel* m_model;

public:
	int instanceID;
	InstanceHandle(InstancedModel* model, int instanceId);
};

