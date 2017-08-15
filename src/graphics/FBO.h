#pragma once

#include <GL/glew.h>

#include "resources/Texture.h"
#include "core/Time.h"

class FBO
{
public:
	FBO();
	~FBO();

	void clear();
	void bind();
	void unbind();

	virtual void init();
	virtual void add_depth_texture();
	virtual void add_color_texture();

	GLuint ID;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	vec2i size;

	bool depth;

	Texture* depth_tex;
	Vector<Texture> color_textures;

	struct color_tex_def
	{
		bool type_byte;
	};

	Array<color_tex_def> definitions;
	
protected:
	void check_status();
};

class FBO2D : public FBO
{
public:
	FBO2D(const vec2i &p_size);

	void init() override;

	void add_depth_texture() override;
	void add_color_texture() override;
};

class FBO1D : public FBO
{
public:
	FBO1D(int size);

	void init() override;

	void add_depth_texture() override;
	void add_color_texture() override;
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