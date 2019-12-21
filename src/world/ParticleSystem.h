#pragma once

#include "Sprite.h"
#include "math/Transform.h"
#include "core/ContentManager.h"
#include "world/WorldObject.h"

class ParticleSystem : public WorldObject
{
	OBJ_DEFINITION(ParticleSystem, WorldObject);

public:
	ParticleSystem();
	virtual ~ParticleSystem();

	void init();
	void start();
	void update() override;
	void draw() override;

	static void bind_methods();

private:
	Array<vec3> positions;
	Array<vec3> velocities;
	Array<Color> colors;

	UBO* ubo;
};