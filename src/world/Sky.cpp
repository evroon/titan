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

	shader->bind();
	shader->set_uniform("view", RENDERER->get_final_matrix());
	shader->set_uniform("light_position", vec3(0, 0, 1));
	shader->set_uniform("ambient", Color::FromRGB(vec3i(66, 182, 244)).get_rgb());
	shader->set_uniform("sun_color", vec3(1.0f));
	shader->set_uniform("camera_position", ACTIVE_WORLD->get_active_camera()->get_pos());

	MeshHandler::get_singleton()->get_cube()->bind();
	MeshHandler::get_singleton()->get_cube()->draw();
	MeshHandler::get_singleton()->get_cube()->unbind();

	RENDERER->use_culling();
}
