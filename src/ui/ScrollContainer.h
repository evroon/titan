#pragma once

#include "Slider.h"
#include "Container.h"

class ScrollContainer : public Container
{
public:
	ScrollContainer();
	~ScrollContainer();

	void use_vert_slider(bool value);
	
	void draw();

	void set_position(const vec2 &p_position);

	Slider *vert_slider = NULL;

	void set_use_scissor(bool value);

private:
	rect2 vert_slider_area;

	rect2 visible_area;

	vec2 position;

	vec2 offset;

	bool use_scissor = true;
};

