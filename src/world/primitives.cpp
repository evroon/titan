#include "primitives.h"

#include "graphics/renderer.h"


void Primitives::Init()
{

}

void Primitives::Destroy()
{

}

void Primitives::DrawRectangle(Shader* shader, const Transform &trans)
{

}

void Primitives::DrawBox(const Transform &trans)
{

}

void Primitives::DrawBoxDark(const Transform &trans)
{

}

void Primitives::DrawBoxLight(const Transform &trans)
{

}

void Primitives::DrawCircle(Shader* shader, const Transform &trans)
{

}

Plane::Plane()
{
	shader = CONTENT->LoadShader("engine/shaders/Shader3D");
	mesh = MeshHandler::get_singleton()->get_plane();
}

void Plane::draw()
{
	Transform t = get_transform();
	t.update();

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("model", t.get_model());
	shader->set_uniform("color", get_color());

	mesh->bind();
	mesh->draw();
	mesh->unbind();

	shader->set_uniform("color", Color::White);
}
