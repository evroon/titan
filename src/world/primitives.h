#pragma once

#include "mesh.h"
#include "sprite.h"
#include "resources/texture.h"
#include "core/contentmanager.h"

#include "worldobject.h"

#include "math/color.h"

class Primitives
{
public:
	//methods
	static void Init();
	static void Destroy();

	static void DrawRectangle(Shader *shader, const Transform &trans);
	static void DrawBox(const Transform &trans);
	static void DrawBoxDark(const Transform &trans);
	static void DrawBoxLight(const Transform &trans);
	static void DrawCircle(Shader *shader, const Transform &trans);
};

class Plane : public WorldObject
{
	OBJ_DEFINITION(Plane, WorldObject)

public:
	Plane();

	void draw() override;

private:
	Shader* shader;
	SimpleMesh* mesh;
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
		shader->bind();
		shader->set_uniform("color", get_color());
		Primitives::DrawRectangle(shader, get_transform());
	}

	void SimpleDraw() override
	{
		CONTENT->SimpleShader->bind();
		shader->set_uniform("color", Color::White);
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
		shader->bind();
		shader->set_uniform("color", get_color());
		Primitives::DrawCircle(shader, get_transform());
	}

	void SimpleDraw() override
	{
		shader->bind();
		shader->set_uniform("color", Color::White);
		Primitives::DrawCircle(shader, get_transform());
	}

	Shader *shader;
};