#pragma once

#include <GL/glew.h>

#include "core/TMessage.h"
#include "Platform.h"

class Linux : public Platform
{
public:
	void InitGL() override
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
			T_ERROR(String("GLEW Error: " + err));
	}
};