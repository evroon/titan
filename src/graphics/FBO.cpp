#include "FBO.h"

#include "FBOManager.h"

#include "Time.h"

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
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		T_ERROR("Error Setting up FBO");
}

void FBO::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);

	glClear(GL_COLOR_BUFFER_BIT);
}

//=========================================================================
//FBO2D
//=========================================================================

FBO2D::FBO2D(const vec2i &p_size)
{
	size = p_size;
	tex = new Texture2D(size);
	CONTENT->AddTexture((Texture2D*)tex);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->ID, 0);
	glDrawBuffers(1, DrawBuffers);

	CheckStatus();
}

//=========================================================================
//FBO1D
//=========================================================================

FBO1D::FBO1D(int size)
{
	this->size = vec2i(size, 1.0f);
	tex = new Texture1D(size);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D, tex->ID, 0);
	glDrawBuffers(1, DrawBuffers);

	CheckStatus();
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
