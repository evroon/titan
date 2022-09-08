#pragma once

#include "core/definitions.h"

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "math/vec2.h"
#include "tmessage.h"

class Window {
   public:
    Window(const vec2i& p_size);

    void Init();
    void SwapBuffer();
    void CreateGLContext();

    vec2i size;
    SDL_Window* window;
};
