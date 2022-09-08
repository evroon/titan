#include "fbomanager.h"

#include "core/windowmanager.h"

FBOManager FBOManager::singleton;

void FBOManager::set_active_fbo(FBO *p_active) { active_fbo = p_active; }

void FBOManager::register_fbo(FBO *fbo) { fbos.push_back(fbo); }

void FBOManager::bind_default_fbo() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vec2i size = WINDOWSIZE;

    glViewport(0, 0, size.x, size.y);
}

void FBOManager::clear_all() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int c = 0; c < fbos.size(); c++)
        if (fbos[c]->cleared_every_frame) fbos[c]->clear();
}

void FBOManager::free() { fbos.clean(); }

FBOManager *FBOManager::get_singleton() { return &singleton; }
