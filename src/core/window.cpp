#include "window.h"

#include "windowmanager.h"

Window::Window(const vec2i& p_size) : size(p_size) {
    WindowManager::get_window_manager()->register_window(this);
    Init();
}

void Window::Init() {
    window = SDL_CreateWindow("Titan Engine", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, size.x, size.y,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) T_ERROR("Failed to create window");

    // SDL_GL_GetDrawableSize(window, &size.x, &size.y);
}

void Window::CreateGLContext() {
    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);
}

void Window::SwapBuffer() { SDL_GL_SwapWindow(window); }
