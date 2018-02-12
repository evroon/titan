#include "Primitives.h"

#include "graphics/Renderer.h"


void Primitives::Init()
{

}

void Primitives::Destroy()
{

}

void Primitives::DrawRectangle(Shader* shader, Transform &trans)
{

}

void Primitives::DrawBox(Transform &trans)
{

}

void Primitives::DrawBoxDark(Transform &trans)
{

}

void Primitives::DrawBoxLight(Transform &trans)
{

}

void Primitives::DrawCircle(Shader* shader, Transform &trans)
{

}

Plane::Plane()
{
	shader = CONTENT->LoadShader("EngineCore/Shaders/Shader3D");
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
