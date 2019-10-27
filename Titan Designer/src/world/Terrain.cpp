#include "Terrain.h"

#include "core/ContentManager.h"

#include "graphics/Renderer.h"
#include "graphics/View.h"

#include "Sky.h"
#include "Model.h"

TerrainBrush::TerrainBrush(Terrain* p_terrain)
{
	terrain = p_terrain;

	heightmap_fbo = new FBO2D(vec2i(4096));
	heightmap_fbo->add_float_color_texture();
	heightmap_fbo->clear_color = vec3(0.0f);
	heightmap_fbo->cleared_every_frame = false;
	heightmap_fbo->init();
	heightmap_fbo->clear();

	terrain->heightmap = heightmap_fbo->color_textures[0]->cast_to_type<Texture2D*>();

	brush_shader = CONTENT->LoadShader("EngineCore/Shaders/Brush");

	to_apply = false;
	active_tex = 0;

	set_radius(100.0f);
	set_strength(.01f);
	
	textures.push_back(CONTENT->LoadTexture("Textures/Brushes/default.png"));
}

void TerrainBrush::apply()
{
	to_apply = true;
}

void TerrainBrush::handle()
{
	if (!to_apply)
		return;

	Transform t = Transform(pos / (128.0f * 10.0f * 0.5f), vec2(radius / (128.0f * 10.0f * 0.5f)));

	Texture2D* heightmap = terrain->get_heightmap();

	brush_shader->bind();
	textures[active_tex]->bind(0);
	heightmap_fbo->bind();

	brush_shader->set_uniform("size", heightmap->get_size());
	brush_shader->set_uniform("strength", strength);
	brush_shader->set_uniform("model", t.get_model());

	DEFERRED_RENDERER->use_additive_blending();

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();

	DEFERRED_RENDERER->use_blending();

	to_apply = false;
}

void TerrainBrush::set_radius(float p_radius)
{
	radius = p_radius;
	terrain->shader->bind();
	terrain->shader->set_uniform("selection_radius", radius);
}

float TerrainBrush::get_radius() const
{
	return radius;
}

void TerrainBrush::set_deviation(float p_deviation)
{
	deviation = p_deviation;
}

float TerrainBrush::get_deviation() const
{
	return deviation;
}

void TerrainBrush::set_color(const Color & p_color)
{
	color = p_color;
	terrain->shader->bind();
	terrain->shader->set_uniform("selection_color", color);
}

Color TerrainBrush::get_color() const
{
	return color;
}

void TerrainBrush::set_strength(float p_strength)
{
	strength = p_strength;
}

float TerrainBrush::get_strength() const
{
	return strength;
}

void TerrainBrush::set_pos(const vec2 & p_pos)
{
	pos = p_pos;
	terrain->shader->bind();
	terrain->shader->set_uniform("selection_pos", pos);
}

vec2 TerrainBrush::get_pos() const
{
	return pos;
}

FBO2D* TerrainBrush::get_fbo() const
{
	return heightmap_fbo;
}

#undef CLASSNAME
#define CLASSNAME TerrainBrush

void TerrainBrush::bind_methods()
{
	//REG_CSTR(0);

	REG_PROPERTY(pos);
	REG_PROPERTY(strength);
	REG_PROPERTY(deviation);
	REG_PROPERTY(radius);
}

//=========================================================================
//Terrain
//=========================================================================

Terrain::Terrain()
{
	height_factor = 1.0f;

	node_count = vec2i(128, 128);
	build();

	shader = CONTENT->LoadShader("EngineCore/Shaders/TerrainEditor");
	shader->bind();
	shader->set_uniform("selection_width", 2.0f);

	normal_map_compute = CONTENT->LoadShader("EngineCore/Shaders/NormalMap");

	Color col = Color::LightBlue;
	col.a = 0.8f;

	brush = new TerrainBrush(this);
	brush->set_color(col);
	brush->set_pos(vec2(0.0f, 0.0f));

	textures.push_back(CONTENT->LoadTexture("Textures/tile.png"));
	textures.push_back(CONTENT->LoadTexture("Textures/Ground_11_DIF.jpg"));
	//textures.push_back(CONTENT->LoadTexture("Textures/look_up.jpg"));

	texture_names.push_back("grass");
	texture_names.push_back("rocks");
	texture_names.push_back("look_up");

	for (int c = 0; c < textures.size(); c++)
	{
		textures[c]->bind(0);
		textures[c]->set_filter(Texture::TRILINEAR_FILTER);
		shader->set_uniform(texture_names[c], c);
	}

	int N = 4096;
	unsigned id = 0;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexStorage2D(GL_TEXTURE_2D, Math::log((float) N) / Math::log(2.0f), GL_RGBA32F, N, N);

	normalmap = new Texture2D(vec2(N, N), (int)id);

	setup_buffers();
	compute_normals();
}


Terrain::~Terrain()
{
}

void Terrain::build()
{
	for (int x = -node_count.x / 2; x < node_count.x / 2; x++)
	{
		for (int y = -node_count.y / 2; y < node_count.y / 2; y++)
		{
			TerrainNode n;
			n.position = vec2(x, y);
			nodes.push_back(n);
		}
	}
}

void Terrain::compute_normals()
{
	int N = normalmap->get_size().x;

	heightmap->bind(0);

	normal_map_compute->bind();
	normal_map_compute->set_uniform("N", N);
	normal_map_compute->set_uniform("strength", 12.0f);
	normal_map_compute->set_uniform("heightmap", 0);

	glBindImageTexture(0, normalmap->get_id(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(N / 16, N / 16, 1);
	glFinish();
}

void Terrain::increase(float p_amount)
{
}

void Terrain::smooth(float p_amount)
{

}

void Terrain::update_buffer()
{

}

void Terrain::update_buffer_range(const vec2& p_start, const vec2 p_end)
{

}

void Terrain::setup_buffers()
{
	struct Vertex
	{
		vec3 position;
	};
	struct Face
	{
		GLubyte index[3];
	};

	int vertices_count = 16;
	int index = 0;

	Vertex* vertices = new Vertex[vertices_count];

	vertices[index++] = { vec3(0, 0, 0) };
	vertices[index++] = { vec3(1.0f / 3.0f, 0, 0) };
	vertices[index++] = { vec3(2.0f / 3.0f, 0, 0) };
	vertices[index++] = { vec3(1.0f, 0, 0) };

	vertices[index++] = { vec3(0, 1.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f / 3.0f, 1.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(2.0f / 3.0f, 1.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f, 1.0f / 3.0f, 0.0f) };

	vertices[index++] = { vec3(0, 2.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f / 3.0f, 2.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(2.0f / 3.0f, 2.0f / 3.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f, 2.0f / 3.0f, 0.0f) };

	vertices[index++] = { vec3(0, 1.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f / 3.0f, 1.0f, 0.0f) };
	vertices[index++] = { vec3(2.0f / 3.0f, 1.0f, 0.0f) };
	vertices[index++] = { vec3(1.0f, 1.0f, 0.0f) };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glPatchParameteri(GL_PATCH_VERTICES, vertices_count);

	glBindVertexArray(0);
}

float Terrain::get_height(const vec2& p_pos) const
{
	return 0.0f;
}

float Terrain::get_height_fast(const vec2& p_pos) const
{
	return 0.0f;
}

Ref<Texture2D> Terrain::get_heightmap() const
{
	return heightmap;
}

TerrainBrush* Terrain::get_brush() const
{
	return brush;
}

void Terrain::set_selection_pos(const vec2& p_pos)
{
	brush->set_pos(p_pos);
}

void Terrain::draw()
{
	//brush->apply();
	//apparently not needed on some intel drivers  
	//glEnable(GL_CLIP_DISTANCE0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("camera_pos", ACTIVE_WORLD->get_active_camera()->get_pos());

	for (int c = 0; c < textures.size(); c++)
	{
		textures[c]->bind(c);
	}

	shader->set_uniform("heightmap", textures.size());
	heightmap->bind(textures.size());
	heightmap->set_filter(Texture::BILINEAR_FILTER);

	shader->set_uniform("normalmap", textures.size() + 1);
	normalmap->bind(textures.size() + 1);

	shader->set_uniform("virtualtex", textures.size() + 2);
	DEFERRED_RENDERER->get_texture(DEFERRED_RENDERER->VIRTUALTEX)->bind(textures.size() + 2);

	shader->set_uniform("indirection", textures.size() + 3);
	DEFERRED_RENDERER->get_texture(DEFERRED_RENDERER->INDIRECTION)->bind(textures.size() + 3);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	for (int c = 0; c < nodes.size(); c++)
	{
		shader->set_uniform("offset", nodes[c].position);

		glDrawArrays(GL_PATCHES, 0, 16);
	}

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}

void Terrain::set_tex_name(int p_index, const String& p_tex_name)
{
	texture_names[p_index] = p_tex_name;
}

#undef CLASSNAME
#define CLASSNAME Terrain

void Terrain::bind_methods()
{
	REG_CSTR(0);
}

//=========================================================================
//Water
//=========================================================================

Water::Water()
{
	set_size(vec3(1000.0f, 1000.0f, 0.0f));
	set_pos(vec3(0, 0, 12.0f));

	shader = CONTENT->LoadShader("EngineCore/Shaders/Water");
	normals = CONTENT->LoadTexture("Textures/oreon/dudv.jpg");
}

void Water::draw()
{
	normals->bind(0);
	DEFERRED_RENDERER->get_texture(DeferredRenderer::REFLECTION)->bind(1);

	Sky* sky = ACTIVE_WORLD->get_child_by_type<Sky*>();
	Camera* cam = ACTIVE_WORLD->get_active_camera();

	set_pos(vec3(cam->get_pos().get_xy(), 6.0f));

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("ambient", Color::FromRGB(vec3i(66, 173, 244)).get_rgb());
	shader->set_uniform("time", TIME->get_absolutetime() / 1000.0f);
	shader->set_uniform("normals", 0);
	shader->set_uniform("reflection_tex", 1);

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();
}

#undef CLASSNAME
#define CLASSNAME Water

void Water::bind_methods()
{
	REG_CSTR(0);
}

//=========================================================================
//Vegetation
//=========================================================================

Vegetation::Vegetation() : Vegetation(NULL)
{

}

Vegetation::Vegetation(Terrain* p_parent)
{
	terrain = p_parent;
	init();
}

void Vegetation::init()
{
	update_buffer();
}

void Vegetation::draw()
{
	Terrain* terrain = ACTIVE_WORLD->get_child_by_type<Terrain*>();
	if (!terrain)
		return;

	RENDERER->stop_culling();
	grass_tex->bind(0);
	terrain->get_heightmap()->bind(1);

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("tex", 0);
	shader->set_uniform("heightmap", 1);

	vec2 cam_pos = ACTIVE_WORLD->get_active_camera()->get_pos().get_xy();
	vec3 pos = vec3(((int)(cam_pos.x / 5.0f)) * 5.0f, ((int)(cam_pos.y / 5.0f)) * 5.0f, 0.0f);

	for (int x = -8; x <= 8; x++)
	{
		for (int y = -8; y <= 8; y++)
		{
			vec3 delta = vec3(x * 5.0f, y * 5.0f, 0.0f);
			float dist = sqrtf(x*x + y * y);
			int lod = 0;

			if (dist < 2)
				lod = 6;
			else if (dist < 4)
				lod = 5;
			else if (dist < 6)
				lod = 4;
			else if (dist < 8)
				lod = 3;
			else if (dist < 10)
				lod = 2;
			else if (dist < 12)
				lod = 1;

			shader->set_uniform("block_offset", delta + pos);

			MeshHandler::get_singleton()->get_plane()->bind();
			MeshHandler::get_singleton()->get_plane()->draw_instanced(2048 / 6 * lod);
		}
	}

	RENDERER->use_culling();
}

void Vegetation::update_buffer()
{
	grass_tex = CONTENT->LoadTexture("Textures/grass_0.png");
	shader = CONTENT->LoadShader("Shaders/Grass/Grass");

	UBO* ubo = new UBO;

	int instances = 2048;
	int sprites = 2 * instances;
	int floats = 4 * sprites;

	for (int c = 0; c < instances; c++)
	{
		vec3 pos;

		float r1 = Math::random() * 5.0f;
		float r2 = Math::random() * 5.0f;
		pos = vec3(r1, r2, 0.0f);

		positions.push_back(pos.x);
		positions.push_back(pos.y);
		positions.push_back(pos.z);
		positions.push_back(0.0f);

		positions.push_back(pos.x);
		positions.push_back(pos.y);
		positions.push_back(pos.z);
		positions.push_back(0.0f);
	}

	ubo->set_data(positions);
	ubo->bind_at_index(0);

	shader->bind();
	shader->bind_block("matrix_block", ubo);
}

void Vegetation::set_terrain(Terrain* p_terrain)
{
	terrain = p_terrain;
}

Terrain* Vegetation::get_terrain() const
{
	return terrain;
}

#undef CLASSNAME
#define CLASSNAME Vegetation

void Vegetation::bind_methods()
{
	REG_CSTR(0);
}

//=========================================================================
//Clouds
//=========================================================================

#include "math\Noise.h"

Clouds::Clouds()
{
	PerlinNoise n;
	texture = n.create_3d_texture();
}

#undef CLASSNAME
#define CLASSNAME Clouds

void Clouds::bind_methods()
{
	REG_CSTR(0);
}