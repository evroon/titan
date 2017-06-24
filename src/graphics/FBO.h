#pragma once

#include <GL/glew.h>

#include "resources/Texture.h"
#include "core/Time.h"

class FBO
{
public:
	FBO();
	~FBO();

	void CheckStatus();
	void Clear();

	GLuint ID;
	Texture* color;
	Texture* depth;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	vec2i size;
	bool depth_buffer;
};

class FBO2D : public FBO
{
public:
	FBO2D(const vec2i &p_size);
};

class FBO1D : public FBO
{
public:
	FBO1D(int size);
};

class RenderTarget
{
public:
	RenderTarget();

	bool should_update();

private:
	bool is_default_target;
	bool fps_locked;
	bool clear_buffer;

	int fps_lock;
	int fps_measured;
	int fps_guessed;

	float delta_time;

	FPSLimiter limiter;
};