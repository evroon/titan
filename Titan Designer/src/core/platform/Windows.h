#pragma once

#if 0
#include "core/platform/dirent.h"
#include <GL\glew.h>

#include "resources/File.h"
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

	File get_cwd()
	{
		char buffer[256];
		unsigned int len = 256;

		int bytes = GetModuleFileName(NULL, buffer, len);
		if (bytes == 0)
		{
			T_ERROR("Could not find directory, error: " + GetLastError());
		}

		File base_path = exepath;
		base_path.go_up(3);
		
        return base_path;
	}
};

#endif