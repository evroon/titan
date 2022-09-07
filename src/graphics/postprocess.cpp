#include "postprocess.h"

#include "core/windowmanager.h"

#include "graphics/renderer.h"
#include "world/terrain.h"

#include "graphics/view.h"

PostProcess::PostProcess() : PostProcess(CONTENT->LoadShader("engine/shaders/PostProcess"))
{

}

PostProcess::PostProcess(Shader* p_shader)
{
	shader = p_shader;
}

PostProcess::~PostProcess()
{
}

void PostProcess::set_area(const rect2& p_area)
{
	area = p_area;
}

rect2 PostProcess::get_area() const
{
	return area;
}

void PostProcess::set_fbo(FBO2D* p_fbo)
{
	fbo = p_fbo;
}

FBO2D* PostProcess::get_fbo() const
{
	return fbo;
}

void PostProcess::post_process()
{
	fbo->color_textures[0]->bind(0);

	FBOMANAGER->bind_default_fbo();
	RENDERER->stop_depth_test();

	shader->bind();

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();
	MeshHandler::get_singleton()->get_plane()->unbind();
}

//=========================================================================
//WorldPostProcess
//=========================================================================

WorldPostProcess::WorldPostProcess()
{
}

WorldPostProcess::~WorldPostProcess()
{
}

void WorldPostProcess::post_process()
{
	FORWARD_RENDERER->get_render_buffer()->color_textures[0]->bind(0);

	Clouds* c = VIEW->get_active_viewport()->get_world()->get_worldobject("clouds")->cast_to_type<Clouds*>();
	//c->texture->bind(1);

	Camera* cam = VIEW->get_active_viewport()->get_world()->get_active_camera();

	FBOMANAGER->bind_default_fbo();
	RENDERER->stop_depth_test();

	shader->bind();
	shader->set_uniform("rendered", 0);
	shader->set_uniform("noise", 1);
	shader->set_uniform("cam_pos", cam->get_pos());

	mat4 inv_proj = cam->get_projection_matrix().inverse();
	mat4 inv_view = cam->get_view_matrix().inverse();

	shader->set_uniform("inv_proj", inv_proj);
	shader->set_uniform("inv_view", inv_view);

	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();
	MeshHandler::get_singleton()->get_plane()->unbind();

	//RENDERER->use_depth_test(0.5f, 1000.0f);
}

//=========================================================================
//WorldPostProcess
//=========================================================================

BlurPostProcess::BlurPostProcess()
{
	blurshader = CONTENT->LoadShader("engine/shaders/Blur");
}

BlurPostProcess::~BlurPostProcess()
{
}

void BlurPostProcess::post_process()
{
#if 0
	blurbuffer->Clear();
	FBOMANAGER->BindFBO(blurbuffer);
	p_tex->Bind(0);

	blurshader->Bind();
	blurshader->setUniform("tex", 0);
	blurshader->setUniform("step", vec2(2.0f / WINDOWSIZE.x, 2.0f / WINDOWSIZE.y));
	blurshader->setUniform("direction", vec2(1, 0));

	//First Pass
	box->shader = blurshader;
	box->get_transform().update(vec3(), blurbuffer->size);
	box->draw();

	//Second Pass
	FBOMANAGER->BindFBO(target);
	blurbuffer->tex->Bind(0);
	blurshader->setUniform("direction", vec2(0, 1));

	box->get_transform().update(vec3(), target->size);
	box->draw();
#endif
}
