#pragma once

#include "math/Transform.h"
#include "Primitives.h"

class Light
{
public:
	Light(Shader* shader, Texture * tex, vec2 pos, double radius);
	Light(Shader * shader, Texture * tex, vec2 pos, double radius, vec4 color);
	~Light();

	void Draw();

	Shader* shader;
	Transform transform;
	Box* box;
	Texture* lightmap;

	vec4 color;
	double radius;
};