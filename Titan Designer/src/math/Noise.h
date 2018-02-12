#pragma once

#include "core/Object.h"

#include "module/perlin.h"
#include "noise.h"

class Texture2D;
class Texture3D;

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

	Texture2D* create_2d_texture();
	Texture3D* create_3d_texture();

private:
	noise::module::Perlin perlin_noise;
};

class WorleyNoise : public Noise
{
	OBJ_DEFINITION(WorleyNoise, Noise);

public:
	WorleyNoise() : WorleyNoise(16.0f) { }
	WorleyNoise(const vec3& p_size);

	float get_value(const vec3& p_pos);

private:
	Array<vec3> spheres;
	vec3 size;
};