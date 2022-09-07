#pragma once

#include "resources/file.h"

class Platform
{
public:
	enum PlatformType
	{
		PLATFORM_UNDEF,
		PLATFORM_WINDOWS,
		PLATFORM_ANDROID,
		PLATFORM_LINUX
	};

	Platform() { }
	~Platform() { }

	virtual void InitSDL() { }
	virtual void InitGL() { }

	virtual File get_cwd() { return ""; }
};
