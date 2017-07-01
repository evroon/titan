#pragma once

#include "core/Component.h"
#include "math/Transform.h"

class TransformComponent : public Component
{
	OBJ_DEFINITION(TransformComponent, Component)

public:
	TransformComponent() { }
	Transform transform;

	operator Transform() { return transform; }

	void update() override;
};