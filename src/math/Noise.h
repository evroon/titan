#pragma once

#include "core/Object.h"

#include "module/perlin.h"
#include "noise.h"

class Noise : public Object
{
	OBJ_DEFINITION(Noise, Object);

public:
	Noise();
	virtual ~Noise();
};

class PerlinNoise : public Noise
{
	OBJ_DEFINITION(PerlinNoise, Noise);

public:
	PerlinNoise();

	float get_value(const vec3& p_pos);

private:
	noise::module::Perlin perlin_noise;
};