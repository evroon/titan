#include "Noise.h"



Noise::Noise()
{
}


Noise::~Noise()
{
}

//=========================================================================
//PerlinNoise
//=========================================================================

PerlinNoise::PerlinNoise()
{

}

float PerlinNoise::get_value(const vec3& p_pos)
{
	return perlin_noise.GetValue(p_pos.x, p_pos.y, p_pos.z);
}
