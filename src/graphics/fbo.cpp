#include "fbo.h"

#include "fbomanager.h"

#include "core/windowmanager.h"

#include "core/time.h"

#include "graphics/renderer.h"
#include "math/color.h"

//=========================================================================
//FBO
//=========================================================================

FBO::FBO()
{
	FBOMANAGER->register_fbo(this);
	clear_color = TO_RGB(vec3i(80, 80, 80));
	depth_tex = nullptr;

	id = 0;
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	size = vec2i();
	depth = false;
	color_textures = Vector<Texture>();
	cleared_every_frame = true;
	definitions = Array<color_tex_def>();
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &id);
}

void FBO::check_status()
{
	int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE)
		T_ERROR("Error Setting up FBO" + String(err));
}

void FBO::clear()
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);

	if (depth && color_textures.size() > 0)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else if (depth)
		glClear(GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}

void FBO::bind()
{
	FBOMANAGER->set_active_fbo(this);

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, size.x, size.y);
}

void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vec2i size = WINDOWSIZE;

	glViewport(0, 0, size.x, size.y);
}

void FBO::init()
{

}

void FBO::add_depth_texture()
{

}

void FBO::add_color_texture()
{

}

void FBO::add_float_color_texture()
{

}

//=========================================================================
//FBO2D
//=========================================================================

FBO2D::FBO2D(const vec2i &p_size)
{
	size = p_size;
	depth = false;
}

FBO2D::FBO2D(const Ref<Texture2D>& p_texture)
{
	size = vec2i(to_int(p_texture->get_size().x), to_int(p_texture->get_size().y));
	depth = false;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	color_textures.push_back(p_texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_texture->get_id(), 0);

	check_status();

	RENDERER->check_error();
}

void FBO2D::init()
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	if (definitions.size() > 0)
	{
		GLenum* DrawBuffers = new GLenum[definitions.size()];
		for (int c = 0; c < definitions.size(); c++)
			DrawBuffers[c] = GL_COLOR_ATTACHMENT0 + c;
		glDrawBuffers(definitions.size(), DrawBuffers);
		delete[] DrawBuffers;
	}
	else
		glDrawBuffer(GL_NONE);


	glReadBuffer(GL_NONE);

	for (int c = 0; c < definitions.size(); c++)
	{
		Texture2D* color;
		if (definitions[c].tex) {
			color = definitions[c].tex;
		} else {
			color = new Texture2D(definitions[c].size, definitions[c].type_byte);
		}
		color_textures.push_back(color);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + c, GL_TEXTURE_2D, color->get_id(), 0);
	}

	if (depth)
	{
		depth_tex = new DepthTexture2D(size);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex->get_id(), 0);
	}

	check_status();

	RENDERER->check_error();
}

Color FBO2D::read_pixel(const vec2& p_pos, int p_attachment_index)
{
	void* data = new float[4];

	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + p_attachment_index);
	glReadPixels(to_int(p_pos.x), to_int(p_pos.y), 1, 1, GL_RGBA, GL_FLOAT, data);

	float* b = (float*)data;

	if (!b)
		return Color();

	Color c = Color(b[0], b[1], b[2], b[3]);

	return c;
}

void FBO2D::add_depth_texture()
{
	depth = true;
}

void FBO2D::add_color_texture()
{
	definitions.push_back({ nullptr, true, size });
}

void FBO2D::add_color_texture(const vec2i& p_size)
{
	definitions.push_back({ nullptr, true, p_size });
}

void FBO2D::add_float_color_texture()
{
	definitions.push_back({ nullptr, false, size });
}

void FBO2D::add_texture(Texture2D* p_texture)
{
	vec2 size = p_texture->get_size();
	definitions.push_back({ p_texture, false, vec2i(size.x, size.y) });
}

//=========================================================================
//FBO1D
//=========================================================================

FBO1D::FBO1D(int size)
{
	this->size = vec2i(size, 1);
	Texture1D* color = new Texture1D(size);

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D, color->get_id(), 0);
	glDrawBuffers(1, DrawBuffers);

	check_status();
}

void FBO1D::init()
{
}

void FBO1D::add_depth_texture()
{
}

void FBO1D::add_color_texture()
{
}

void FBO1D::add_float_color_texture()
{
}
