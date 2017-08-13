#include "FBO.h"

#include "FBOManager.h"

#include "Time.h"

#include "graphics/Renderer.h"

//=========================================================================
//FBO
//=========================================================================

FBO::FBO()
{
	FBOMANAGER->AddFBO(this);
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &ID);
}

void FBO::CheckStatus()
{
	int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE)
		T_ERROR("Error Setting up FBO" + String(err));
}

void FBO::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);

	if (depth && color_textures.size() > 0)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else if (depth)
		glClear(GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
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

//=========================================================================
//FBO2D
//=========================================================================

FBO2D::FBO2D(const vec2i &p_size)
{
	size = p_size;
	depth = false;
}

void FBO2D::init()
{
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glClearColor(0.3f, 0.3f, 0.3f, 1);

	if (definitions.size() > 0)
	{
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
	}
	else
		glDrawBuffer(GL_NONE);


	glReadBuffer(GL_NONE);

	for (int c = 0; c < definitions.size(); c++)
	{
		Texture2D* color = new Texture2D(size, definitions[c].type_byte);
		color_textures.push_back(color);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + c, GL_TEXTURE_2D, color->get_id(), 0);
	}

	if (depth)
	{
		depth_tex = new DepthTexture2D(size);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex->get_id(), 0);
	}

	CheckStatus();

	RENDERER->CheckGLError();
}

void FBO2D::add_depth_texture()
{
	depth = true;
}

void FBO2D::add_color_texture()
{
	definitions.push_back({ true });
}

//=========================================================================
//FBO1D
//=========================================================================

FBO1D::FBO1D(int size)
{
	this->size = vec2i(size, 1);
	Texture1D* color = new Texture1D(size);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D, color->get_id(), 0);
	glDrawBuffers(1, DrawBuffers);

	CheckStatus();
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

//=========================================================================
//RenderTarget
//=========================================================================

RenderTarget::RenderTarget()
{
	fps_locked = true;
	fps_lock = 60;
	is_default_target = true;
	clear_buffer = true;

	limiter = FPSLimiter(1000 / fps_lock);
}

bool RenderTarget::should_update()
{
	if (fps_locked)
	{
		FPSLimiter::FPSInfo i = limiter.update();

		if (i.passedsec)
		{
			fps_measured = i.FPS;
			
		}

		if (i.needsupdate)
		{
			delta_time = i.delta_time;

			//T_LOG(delta_time);
		}

		return i.needsupdate;
	}
	else
		return true;
}
