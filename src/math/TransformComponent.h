#pragma once

#include "Component.h"
#include "Transform.h"

class TransformComponent : public Component
{
	OBJ_DEFINITION(TransformComponent, Component)

public:
	TransformComponent() { }
	Transform transform;

	operator Transform() { return transform; }

	void update() override;
};