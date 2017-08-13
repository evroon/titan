#pragma once

#include "WorldObject.h"

#include "Mesh.h"


class Terrain :	public WorldObject
{
	OBJ_DEFINITION(Terrain, WorldObject);

public:
	Terrain();
	virtual ~Terrain();

	void build_from_heightmap(RawTexture2D* p_heightmap);
	void generate_normals();
	void setup_buffers();

	float get_height(const vec2& p_pos) const;
	float get_height_fast(const vec2& p_pos) const;

	void draw() override;

	void set_tex_name(int p_index, const String & p_tex_name);

	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texcoords;
	};

	struct Face
	{
		unsigned indices[3];
	};

private:
	vec2i size;

	GLuint VAO, VBO, EBO;

	Array<Face> faces;
	Array<Vertex> vertices;

	Shader* shader;

	Texture2D* look_up;

	Vector<Texture2D> textures;
	Array<String> texture_names;

	Color diffuse_color;

	float height_factor;
};

class Water : public WorldObject
{
	OBJ_DEFINITION(Water, WorldObject);

public:
	Water();

	void draw() override;

private:
	Shader* shader;

	Texture2D* normals;
};

class Vegetation : public WorldObject
{
	OBJ_DEFINITION(Vegetation, WorldObject);

public:
	Vegetation(Terrain* p_parent);

	void draw() override;

private:
	Shader* shader;
	Texture2D* grass_tex;
	Terrain* terrain;

	Array<mat4> model_matrices;
};

class Clouds : public WorldObject
{
	OBJ_DEFINITION(Clouds, WorldObject);

public:
	Clouds();


	Texture3D* texture;
	Shader* shader;
};