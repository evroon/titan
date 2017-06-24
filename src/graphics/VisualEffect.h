#pragma once

#include "FBO.h"
#include "resources/Shader.h"
#include "FBOManager.h"
#include "core/ContentManager.h"
#include "world/Primitives.h"

#define VISUALEFFECT VisualEffect::get_singleton()

class VisualEffect : public Object
{
	OBJ_DEFINITION(VisualEffect, Object);

public:
	VisualEffect();
	virtual ~VisualEffect();

	void init();
	void free();
	void post_process();

	void blur_texture(Texture* p_tex, FBO2D* p_target);
	
	Shader* shader;
	Shader* blurshader;

	FBO2D *blurbuffer;

	static VisualEffect* get_singleton();

private:
	static VisualEffect* singleton;
};