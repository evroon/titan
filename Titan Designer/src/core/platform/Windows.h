#pragma once

#if 0
#include <GL\glew.h>

#include "TMessage.h"
#include "Platform.h"

class Windows : public Platform
{
public:
	void InitGL() override
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
			T_ERROR(String("GLEW Error: " + err));
	}
};

#endif