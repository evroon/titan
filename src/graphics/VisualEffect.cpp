#include "VisualEffect.h"

#include "core/WindowManager.h"

#include "graphics/Renderer.h"
#include "world/Terrain.h"

#include "graphics/View.h"

VisualEffect* VisualEffect::singleton;

VisualEffect::VisualEffect()
{
	blurshader = CONTENT->LoadShader("EngineCore/Blur");
	shader = CONTENT->LoadShader("EngineCore/PostProcess");

	//blurbuffer = new FBO2D(size);
}

VisualEffect::~VisualEffect()
{
}

void VisualEffect::init()
{
	singleton = new VisualEffect;
}

void VisualEffect::free()
{

}

void VisualEffect::post_process()
{
	//BlurTexture(FBOMANAGER->ShadowFBO->tex, FBOMANAGER->ShadowFBO);

	RENDERER->get_render_buffer()->color_textures[0]->bind(0);

	Clouds* c = VIEW->get_active_viewport()->get_world()->get_worldobject("clouds")->cast_to_type<Clouds*>();
	c->texture->bind(1);

	Camera* cam = VIEW->get_active_viewport()->get_world()->get_active_camera();

	FBOMANAGER->BindDefaultFBO();

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

	RENDERER->use_depth_test(0.5f, 1000.0f);
}

void VisualEffect::blur_texture(Texture *p_tex, FBO2D *p_target)
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

VisualEffect * VisualEffect::get_singleton()
{
	return singleton;
}
