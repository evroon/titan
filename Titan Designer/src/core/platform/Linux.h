#pragma once

#include <GL/glew.h>

#include <unistd.h>
#include <limits.h>

#include "resources/File.h"
#include "core/TMessage.h"
#include "Platform.h"

class Linux : public Platform
{
public:
	void InitGL() override
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
			T_ERROR("GLEW Error: " + String(err));
	}

	File get_cwd() override
	{
        char arg1[20];
        char exepath[PATH_MAX + 1] = {0};

        sprintf(arg1, "/proc/%d/exe", getpid());
        readlink(arg1, exepath, sizeof(exepath));
		
		File base_path = exepath;
		base_path.go_up(3);

        return base_path;
	}
};