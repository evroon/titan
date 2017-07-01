#pragma once

#include "Sprite.h"
#include "math/Transform.h"
#include "core/ContentManager.h"
#include "world/WorldObject.h"

class Particle
{
public:
	Particle(vec2 p) { pos = p; }
	Particle(vec2 p, vec4 c) { pos = p; color = c; }

	int lifetime = 0;	//in ms

	vec2 pos;
	vec4 color;

	double velocity;
};

class ParticleSystem : public WorldObject
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	void update() override;
	void draw() override;

	Sprite *sprite;
	Shader *shader = CONTENT->Shader2D;

private:
	Array<Particle> particles;
};