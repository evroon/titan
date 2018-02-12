#pragma once

#include "WorldObject.h"

class PointLight : public WorldObject
{
	OBJ_DEFINITION(PointLight, WorldObject);

public:
	PointLight();
	virtual ~PointLight();

	static void bind_methods();

private:
};

class DirectionalLight : public WorldObject
{
	OBJ_DEFINITION(DirectionalLight, WorldObject);

public:
	DirectionalLight();
	virtual ~DirectionalLight();

	static void bind_methods();

private:
};

class ConeLight : public WorldObject
{
	OBJ_DEFINITION(ConeLight, WorldObject);

public:
	ConeLight();
	virtual ~ConeLight();

	static void bind_methods();

private:
};