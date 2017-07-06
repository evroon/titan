#include "Terrain.h"

#include "core/ContentManager.h"

#include "graphics/Renderer.h"
#include "graphics/View.h"

#define INDEX(X, Y) (X) * (size.y + 1) + Y
#define HEIGHT(X, Y) vertices[INDEX(X, Y)].position.z
#define POSITION(X, Y) vertices[INDEX(X, Y)].position

Terrain::Terrain()
{
	size = vec2i(254);
	height_factor = 0.1f;

	build_from_heightmap(CONTENT->LoadRawTexture("Textures/heightmap.png"));

	shader = CONTENT->LoadShader("EngineCore/Terrain");

	textures.push_back(CONTENT->LoadTexture("Textures/grass.png"));
	textures.push_back(CONTENT->LoadTexture("Textures/Ground_11_DIF.jpg"));

	texture_names.push_back("grass");
	texture_names.push_back("rocks");

	//add_child(new Water);
}


Terrain::~Terrain()
{
}

void Terrain::build_from_heightmap(RawTexture2D* p_heightmap)
{
	for (int x = 0; x <= size.x; x++) // size.x + 1 elements
	{
		for (int y = 0; y <= size.y; y++) // size.y + 1 elements
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
			f1.indices[1] = INDEX(x + 1, y);
			f1.indices[2] = INDEX(x, y + 1);

			f2.indices[0] = INDEX(x, y + 1);
			f2.indices[1] = INDEX(x + 1, y);
			f2.indices[2] = INDEX(x + 1, y + 1);

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

		if (c < size.y + 1)
			left = here;
		else
			left = vertices[c - (size.y + 1)].position.z;

		if (c >= vertices.size() - size.y - 1)
			right = here;
		else
			right = vertices[c + (size.y + 1)].position.z;

		if (c % (size.y + 1) == 0)
			down = here;
		else
			down = vertices[c - 1].position.z;

		if (c % (size.y + 1) == size.y)
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

	if (clipped.x >= 1 - pos.y)
	{
		result = Math::get_barry_centric(vec3(0.0f, bl, 0.0f), vec3(1.0f, br, 0.0f), vec3(0, ul, 1), clipped);
	}
	else
	{
		result = Math::get_barry_centric(vec3(1.0f, br, 0.0f), vec3(1.0f, ur, 1.0f), vec3(0, ul, 1), clipped);
	}

	result -=12;

	return result;
}

void Terrain::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//aparantly not needed on some intel drivers
	glEnable(GL_CLIP_DISTANCE0);

	glBindVertexArray(VAO);

	shader->Bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", mat4::Translate(vec3(0, 0, -12)));
	shader->set_uniform("light_matrix", RENDERER->get_light_matrix());
	shader->set_uniform("ambient", vec3(0.4f));
	
	for (int c = 0; c < textures.size(); c++)
	{
		textures[c]->Bind(c);
		shader->set_uniform(texture_names[c], c);
	}

	Texture* depth_tex = RENDERER->get_shadow_buffer()->depth_tex;
	depth_tex->Bind(2);
	shader->set_uniform("shadow_map", 2);

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
	set_size(vec3(1000.0f, 1000.0f, 0.0f));
	set_pos(vec3(0, 0, 0.0f));

	shader = CONTENT->LoadShader("EngineCore/Water");
	normals = CONTENT->LoadTexture("Textures/dudv.jpg");
}

void Water::draw()
{
	normals->Bind(0);
	RENDERER->get_reflection_buffer()->color_textures[0]->Bind(1);

	shader->Bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("ambient", Color::FromRGB(vec3i(66, 173, 244)).get_rgb());
	shader->set_uniform("light_direction", vec3(0, 0, 1));
	shader->set_uniform("light_color", vec3(1.0f));
	shader->set_uniform("camera_position", ACTIVE_WORLD->get_active_camera()->get_pos());
	shader->set_uniform("time", TIME->get_absolutetime() / 1000.0f);

	shader->set_uniform("normals", 0);
	shader->set_uniform("reflection_tex", 1);

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();
}

//=========================================================================
//Vegetation
//=========================================================================

Vegetation::Vegetation()
{
	grass_tex = CONTENT->LoadTexture("Textures/grass_0.png");
	shader = CONTENT->LoadShader("Shaders/Grass/Grass");

	set_pos(vec3(100, 100, 1));
	set_rotation(vec3(PI / 2.0f, 0, 0));
}

void Vegetation::draw()
{
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.01);

	RENDERER->stop_culling();
	grass_tex->Bind(0);

	static Transform t = Transform(vec3(101, 101, 1), vec3(1.0f), vec3(PI / 2.0f, PI / 2.0f, 0));

	shader->Bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("color", vec4(1.0));
	shader->set_uniform("texture_enabled", true);
	shader->set_uniform("ambient", vec3(0.3f));

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();

	shader->set_uniform("model", t.get_model());

	MeshHandler::get_singleton()->get_plane()->draw();

	RENDERER->use_culling();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
