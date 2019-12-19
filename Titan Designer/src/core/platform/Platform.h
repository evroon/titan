#pragma once

#include "resources/File.h"

class Platform
{
public:
	enum PlatformType
	{
		UNDEF,
		WINDOWS,
		ANDROID,
		LINUX
	};

	Platform() { }
	~Platform() { }

	virtual void InitSDL() { }
	virtual void InitGL() { }
	
	virtual File get_cwd() { return ""; }
};

