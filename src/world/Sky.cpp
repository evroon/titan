#include "Sky.h"

#include "Mesh.h"

#include "Core/ContentManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/View.h"

Sky::Sky()
{
	shader = CONTENT->LoadShader("EngineCore/Sky");

	set_size(vec3(10.0f, 10.0f, 10.0f));
	set_pos(vec3(0, 0, 0.0f));
}

Sky::~Sky()
{

}

void Sky::draw()
{
	RENDERER->stop_culling();

	shader->Bind();
	shader->setUniform("view", RENDERER->get_final_matrix());
	shader->setUniform("light_position", vec3(0, 0, 1));
	shader->setUniform("ambient", Color::FromRGB(vec3i(66, 182, 244)).get_rgb());
	shader->setUniform("sun_color", vec3(1.0f));
	shader->setUniform("camera_position", ACTIVE_WORLD->get_active_camera()->get_pos());

	MeshHandler::get_singleton()->get_cube()->bind();
	MeshHandler::get_singleton()->get_cube()->draw();
	MeshHandler::get_singleton()->get_cube()->unbind();

	RENDERER->use_culling();
}
