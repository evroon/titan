#include "Model.h"

#include "graphics/Renderer.h"

#include "World.h"
#include "graphics/View.h"
#include "Sky.h"

Model::Model()
{
	shader = CONTENT->LoadShader("EngineCore/Shaders/Shader3D");
	color_id = vec3(0.0, 1.0, 0.5);
}

Model::Model(const String& p_path) : Model()
{
	load_mesh(p_path);
}

Model::Model(Mesh* p_mesh) : Model()
{
	set_mesh(p_mesh);
}

Model::~Model()
{
}

void Model::draw()
{
	if (!mesh)
		return;

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("color_id", color_id);
	shader->set_uniform("color", get_color());

	mesh->draw();
}

void Model::shadow_draw()
{
	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("color", get_color());
	shader->set_uniform("texture_enabled", true);
	shader->set_uniform("ambient", vec3(0.3f));
	
	mesh->draw();

	shader->set_uniform("color", Color::White);
}

void Model::load_mesh(const String& p_path)
{
	T_LOG("Loading mesh " + p_path);
	set_mesh(new Mesh(p_path));
}

void Model::set_mesh(Mesh* p_mesh)
{
	mesh = p_mesh;
	
	if (mesh)
		mesh->model = this;
}

Mesh* Model::get_mesh() const
{
	return mesh;
}

void Model::set_color_id(const vec3 & p_color_id)
{
	color_id = p_color_id;
}

vec3 Model::get_color_id() const
{
	return color_id;
}

#undef CLASSNAME
#define CLASSNAME Model

void Model::bind_methods()
{
	REG_CSTR(0);

	REG_PROPERTY(mesh);
}
