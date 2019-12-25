#pragma once

#include "WorldObject.h"

class Environment :	public WorldObject
{
	OBJ_DEFINITION(Environment, WorldObject);

public:
	Environment();
	virtual ~Environment();

	void set_ambient_color(const Color& p_ambient_color);
	Color get_ambient_color() const;

	void set_auto_exposure_enabled(bool p_auto_exposure_enabled);
	bool get_auto_exposure_enabled() const;

	void set_fog_enabled(bool p_fog_enabled);
	bool get_fog_enabled() const;

	void set_fog_density(float p_fog_density);
	float get_fog_density() const;

	void set_fog_gradient(float p_fog_gradient);
	float get_fog_gradient() const;

	SIMPLE_GET_SET(bool, dof_enabled);

	SIMPLE_GET_SET(bool, ssao_enabled);
	SIMPLE_GET_SET(float, ssao_radius);

	static void bind_methods();

private:
	//ambient
	Color ambient_color;

	//auto exposure
	bool auto_exposure_enabled;
	float auto_exposure_speed;
	float auto_exposure_max;
	float auto_exposure_min;

	//bloom
	bool bloom_enabled;

	//fog
	bool fog_enabled;
	float fog_density;
	float fog_gradient;
	Color fog_color;

	//dof
	bool dof_enabled;

	//godrays
	bool godrays_enabled;

	//lighting
	bool lighting_enabled;
	bool volumetric_lighting_enabled;

	//tonemapping
	int tonemap_type;
	float exposure;

	// SSAO
	bool ssao_enabled;
	float ssao_radius;
};

