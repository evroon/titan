#pragma once

#include <SDL.h>

#include "math/Vec2.h"

#include "TMessage.h"

class Window
{
public:
	Window(const vec2i &p_size);

	void Init();
	void SwapBuffer();
	void CreateGLContext();

	vec2i size;
	SDL_Window* window;
};

