#include "VisualEffect.h"

#include "core/WindowManager.h"

#include "graphics/Renderer.h"

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

	FBOMANAGER->BindDefaultFBO();

	RENDERER->stop_depth_test();

	shader->Bind();
	shader->set_uniform("rendered", 0);

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
