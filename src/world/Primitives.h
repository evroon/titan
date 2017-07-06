#pragma once

#include "Mesh.h"
#include "Sprite.h"
#include "resources/Texture.h"
#include "core/ContentManager.h"

#include "WorldObject.h"

#include "math/Color.h"

class Primitives
{
public:
	//methods
	static void Init();
	static void Destroy();

	static void DrawRectangle(Shader *shader, Transform &trans);
	static void DrawBox(Transform &trans);
	static void DrawBoxDark(Transform &trans);
	static void DrawBoxLight(Transform &trans);
	static void DrawCircle(Shader *shader, Transform &trans);
};

class Box : public WorldObject
{
	OBJ_DEFINITION(Box, WorldObject)

public:
	Box(const vec3& p_pos, const vec3& p_size)
	{
		shader = CONTENT->SimpleShader;
		get_transform().update(p_pos, p_size);
	}
	Box() : Box(vec3(), vec3(100.0)) { }

	void draw() override
	{
		shader->Bind();
		shader->set_uniform("color", get_color());
		Primitives::DrawRectangle(shader, get_transform());
	}

	void SimpleDraw() override
	{
		CONTENT->SimpleShader->Bind();
		shader->setWhiteColor("color");
		Primitives::DrawRectangle(CONTENT->SimpleShader, get_transform());
	}

	Shader *shader;
};

class Disk : public WorldObject
{
	OBJ_DEFINITION(Disk, WorldObject)

public:
	Disk(const vec3& p_pos, const vec3& p_size)
	{
		shader = CONTENT->Shader2D;
		get_transform().update(p_pos, p_size);
	}
	Disk() : Disk(vec3(), vec3(100.0)) { }

	void draw() override
	{
		shader->Bind();
		shader->set_uniform("color", get_color());
		Primitives::DrawCircle(shader, get_transform());
	}

	void SimpleDraw() override
	{
		shader->Bind();
		shader->setWhiteColor("color");
		Primitives::DrawCircle(shader, get_transform());
	}

	Shader *shader;
};