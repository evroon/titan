#include "Model.h"

#include "graphics/Renderer.h"

Model::Model()
{
	shader = CONTENT->LoadShader("EngineCore/Shader3D");
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
	shader->Bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("color", get_color());
	shader->set_uniform("texture_enabled", true);
	shader->set_uniform("ambient", vec3(0.3f));
	shader->set_uniform("view_pos", get_world()->get_active_camera()->get_pos());

	mesh->draw();

	shader->setWhiteColor("color");
}

void Model::shadow_draw()
{
	shader->Bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", get_transform().get_model());
	shader->set_uniform("color", get_color());
	shader->set_uniform("texture_enabled", true);
	shader->set_uniform("ambient", vec3(0.3f));

	mesh->draw();

	shader->setWhiteColor("color");
}

void Model::load_mesh(const String& p_path)
{
	set_mesh(new Mesh(p_path));
}

void Model::set_mesh(Mesh* p_mesh)
{
	mesh = p_mesh;
}

Mesh* Model::get_mesh() const
{
	return mesh;
}

void Model::set_shader(Shader * p_shader)
{
	shader = p_shader;
}

Shader * Model::get_shader() const
{
	return shader;
}
