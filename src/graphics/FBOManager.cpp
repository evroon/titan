#include "FBOManager.h"

#include "core/WindowManager.h"

FBOManager FBOManager::singleton;

void FBOManager::AddFBO(FBO *fbo)
{
	fbos.push_back(fbo);
}

void FBOManager::Init()
{

}

void FBOManager::BindFBO(FBO *fbo)
{
	activeFBO = fbo;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
	glViewport(0, 0, fbo->size.x, fbo->size.y);
}

void FBOManager::BindDefaultFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vec2i size = WINDOWSIZE;

	glViewport(0, 0, size.x, size.y);
}

void FBOManager::ClearAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int c = 0; c < fbos.size(); c++)
		fbos[c]->Clear();
}

void FBOManager::Destroy()
{
	fbos.clean();
}

FBOManager* FBOManager::get_singleton()
{
	return &singleton;
}
