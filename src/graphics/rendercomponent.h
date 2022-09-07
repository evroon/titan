#pragma once

#include "math/color.h"
#include "core/component.h"

class RenderComponent : public Component
{
	OBJ_DEFINITION(RenderComponent, Component)

public:
	RenderComponent() { }

	bool isvisible = true;
	bool isstatic = false;

	int layer = 0;

	void set_color(const Color& p_color);
	Color get_color() const;

	static void bind_methods();

private:
	Color color = Color::White;
};