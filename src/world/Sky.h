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

	void build_mesh();


	void draw() override;


private:
	Shader* shader;

	unsigned VAO, VBO, EBO;

};

