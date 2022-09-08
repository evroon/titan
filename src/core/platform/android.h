#pragma once

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#else
#include "sdl.h"
#endif

#include "platform.h"

class Android : public Platform
{
public:
	void InitGL() override
	{
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	}
};
