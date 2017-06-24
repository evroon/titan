#pragma once

#include "Container.h"
#include "NumberField.h"

#include "String.h"

class Vec3Field : public Control
{
public:
	Vec3Field();
	Vec3Field(const vec3 &p_vec3);

	vec2 get_required_size() const;

	void handle_event(UIEvent *ui_event) override;

	void draw();

private:
	Container container;
	NumberField x, y, z;
};