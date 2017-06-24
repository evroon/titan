#pragma once

#include "WorldObject.h"

class Mesh;

class Sky :	public WorldObject
{
	OBJ_DEFINITION(Sky, WorldObject);

public:
	Sky();
	virtual ~Sky();


	void draw() override;


private:
	Mesh* mesh;
};

