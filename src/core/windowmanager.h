#pragma once

#include "window.h"

#define WINDOWMAN WindowManager::get_window_manager()
#define WINDOW WindowManager::get_window_manager()->get_default_window()
#define WINDOWSIZE \
    WindowManager::get_window_manager()->get_default_window_size()
#define WINDOWSIZE_F \
    WindowManager::get_window_manager()->get_default_window_size_f()

class WindowManager {
   public:
    WindowManager();
    ~WindowManager();

    static WindowManager *get_window_manager();

    void register_window(Window *window);

    Window *get_default_window();
    vec2i get_default_window_size();
    vec2 get_default_window_size_f();

    void clean();

   private:
    Vector<Window> windows;
    static WindowManager window_manager;

    Window *default_window;
};
