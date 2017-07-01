#include "Terrain.h"

#include "core/ContentManager.h"

#include "graphics/Renderer.h"

#define INDEX(X, Y) (X) * (size.y + 1) + Y
#define HEIGHT(X, Y) vertices[INDEX(X, Y)].position.z
#define POSITION(X, Y) vertices[INDEX(X, Y)].position

Terrain::Terrain()
{
	size = vec2i(255);
	height_factor = 0.1f;

	build_from_heightmap(CONTENT->LoadRawTexture("Textures/heightmap.png"));

	shader = CONTENT->LoadShader("EngineCore/Terrain");

	textures.push_back(CONTENT->LoadTexture("Textures/grass.png"));

	//add_child(new Water);
}


Terrain::~Terrain()
{
}

void Terrain::build_from_heightmap(RawTexture2D* p_heightmap)
{
	for (int x = 0; x <= size.x; x++) // size.x + 1 elements
	{
		for (int y = 0; y <= size.y; y++)
		{
			Vertex v;

			float z = 0.0f;

			if (p_heightmap)
				z = p_heightmap->read_pixel(vec2i(x, y)).r * height_factor;

			v.position = vec3(to_float(x), to_float(y), z);
			v.texcoords = vec2(to_float(x) / size.x, to_float(y) / size.y);
			v.normal = vec3(0, 0, 1);
			
			vertices.push_back(v);
		}
	}

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			Face f1, f2;

			f1.indices[0] = INDEX(x, y);
			f1.indices[1] = INDEX(x, y + 1);
			f1.indices[2] = INDEX(x + 1, y);

			f2.indices[0] = INDEX(x, y + 1);
			f2.indices[1] = INDEX(x + 1, y + 1);
			f2.indices[2] = INDEX(x + 1, y);

			faces.push_back(f1);
			faces.push_back(f2);
		}
	}

	generate_normals();

	setup_buffers();
}

void Terrain::generate_normals()
{
	for (int c = 0; c < vertices.size(); c++)
	{
		int x = c / (size.x + 1);
		int y = c % (size.y + 1);

		vec3 normal;

		//https://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid

		float here = vertices[c].position.z;
		float left, right, up, down;

		if (c <= size.y)
			left = here;
		else
			left = vertices[c - (size.y + 1)].position.z;

		if (c >= vertices.size() - size.y)
			right = here;
		else
			right = vertices[c + (size.y + 1)].position.z;

		if (c % size.y == 0)
			down = here;
		else
			down = vertices[c - 1].position.z;

		if (c == vertices.size() - 1)
			up = here;
		else
			up = vertices[c + 1].position.z;

		normal.x = left - right;
		normal.y = down - up;
		normal.z = 2.0;
		normal = normal.normalize();

		vertices[c].normal = normal;
	}
}

void Terrain::setup_buffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), &faces[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

	glBindVertexArray(0);
}

float Terrain::get_height(const vec2& p_pos) const
{
	vec2 pos = p_pos;

	if (pos.x < 0 || pos.y < 0 || pos.x > size.x || pos.y > size.y)
		return 0.0f;

	float bl = HEIGHT(to_int(pos.x), to_int(pos.y));
	float br = HEIGHT(to_int(pos.x + 1), to_int(pos.y));
	float ul = HEIGHT(to_int(pos.x), to_int(pos.y + 1));
	float ur = HEIGHT(to_int(pos.x + 1), to_int(pos.y + 1));

	float result;

	vec2 clipped = pos - vec2(to_int(pos.x), to_int(pos.y));

	if (clipped.x <= 1 - pos.y)
	{
		result = Math::get_barry_centric(vec3(0.0f, bl, 0.0f), vec3(1.0f, br, 0.0f), vec3(0, ul, 1), clipped);
	}
	else
	{
		result = Math::get_barry_centric(vec3(1.0f, br, 0.0f), vec3(1.0f, ur, 1.0f), vec3(0, ul, 1), clipped);
	}

	return result;
}

void Terrain::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);

	shader->Bind();
	shader->setUniform("view", RENDERER->get_final_matrix());
	shader->setUniform("model", mat4());
	//shader->setUniform("ambient", vec3(0.4f));
	
	textures[0]->Bind(0);


	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Terrain::set_tex_name(int p_index, const String& p_tex_name)
{
	texture_names[p_index] = p_tex_name;
}

//=========================================================================
//Water
//=========================================================================

Water::Water()
{
	set_size(vec3(1000.0f, 1000.0f, 1.0f));
	set_pos(vec3(0, 0, 15.0f));

	shader = CONTENT->LoadShader("EngineCore/Water");
}

void Water::draw()
{
	RENDERER->stop_culling();

	Transform t = get_transform();
	t.update();

	shader->Bind();
	shader->setUniform("view", RENDERER->RENDERER->get_final_matrix());
	shader->setUniform("model", t.get_model());
	shader->setUniform("color", Color::Blue);
	shader->setUniform("light_position", vec3(0, 0, 100));
	shader->setUniform("ambient", vec3(0.3f));

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();

	RENDERER->use_culling();
}
