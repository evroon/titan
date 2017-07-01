#pragma once

#include "math/Color.h"
#include "core/Component.h"

class RenderComponent : public Component
{
	OBJ_DEFINITION(RenderComponent, Component)

public:
	RenderComponent() { }

	bool isvisible = true;
	bool isstatic = false;
	Color color = Color::White;

	int layer = 0;
};