#pragma once

#include "FBO.h"
#include "core/Vector.h"

#define FBOMANAGER FBOManager::get_singleton()

class FBOManager
{
public:
	void set_active_fbo(FBO* p_active);
	void register_fbo(FBO *fbo);
	void bind_default_fbo();
	void clear_all();
	void free();

	static FBOManager* get_singleton();

private:
	FBO* active_fbo;
	Vector<FBO> fbos;
	static FBOManager singleton;
};