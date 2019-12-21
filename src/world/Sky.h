#pragma once

#include "WorldObject.h"

#include "Mesh.h"

class Mesh;

class Sky :	public WorldObject
{
	OBJ_DEFINITION(Sky, WorldObject);

public:
	Sky();
	virtual ~Sky();

	void update() override;
	void draw() override;

	void set_sun_direction(const vec3& p_dir);
	vec3 get_sun_direction() const;

	void set_sun_color(const Color & p_sun_color);
	Color get_sun_color() const;

	void set_sky_color(const Color& p_sky_color);
	Color get_sky_color() const;

	static void bind_methods();

private:
	Shader* shader;

	vec3 sun_direction;
	Color sky_color;
	Color sun_color;
};

