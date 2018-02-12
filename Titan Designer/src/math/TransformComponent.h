#pragma once

#include "core/Component.h"
#include "math/Transform.h"

class TransformComponent : public Component
{
	OBJ_DEFINITION(TransformComponent, Component)

public:
	TransformComponent() { }

	operator Transform() { return transform; }

	void update() override;

	void set_transform(const Transform& p_transform);
	Transform get_transform() const;

	static void bind_methods();

	Transform transform;
};