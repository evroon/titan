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
	virtual void add_float_color_texture();

	GLuint id;
	GLenum DrawBuffers[1];
	vec2i size;

	bool depth;
	Color clear_color;

	Texture* depth_tex;
	Vector<Texture> color_textures;

	struct color_tex_def
	{
		bool type_byte;
		vec2i size;
	};

	bool cleared_every_frame;

	Array<color_tex_def> definitions;
	
protected:
	void check_status();
};

class FBO2D : public FBO
{
public:
	FBO2D(const vec2i &p_size);
	FBO2D(const Ref<Texture2D>& p_texture);

	void init() override;

	Color read_pixel(const vec2& p_pos, int p_attachment_index);

	void add_depth_texture() override;
	void add_color_texture() override;
	void add_color_texture(const vec2i& p_size);
	void add_float_color_texture() override;
};

class FBO1D : public FBO
{
public:
	FBO1D(int size);

	void init() override;

	void add_depth_texture() override;
	void add_color_texture() override;
	void add_float_color_texture() override;
};