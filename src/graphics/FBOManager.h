#pragma once

#include "FBO.h"
#include "core/Vector.h"

#define FBOMANAGER FBOManager::get_singleton()

class FBOManager
{
public:
	void Init();

	void AddFBO(FBO *fbo);

	void BindFBO(FBO *fbo);
	void BindDefaultFBO();

	void ClearAll();

	void Destroy();

	static FBOManager* get_singleton();

	FBO* activeFBO;

private:

	Vector<FBO> fbos;

	static FBOManager singleton;
};