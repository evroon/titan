#include "windowmanager.h"

#include "math/math.h"

WindowManager WindowManager::window_manager;

WindowManager::WindowManager() {}

WindowManager::~WindowManager() {}

WindowManager* WindowManager::get_window_manager() { return &window_manager; }

void WindowManager::register_window(Window* window) {
    default_window = window;
    windows.push_back(window);
}

Window* WindowManager::get_default_window() { return default_window; }

vec2i WindowManager::get_default_window_size() { return get_default_window()->size; }

vec2 WindowManager::get_default_window_size_f() {
    vec2i size = get_default_window_size();
    return vec2(to_float(size.x), to_float(size.y));
}

void WindowManager::clean() { windows.clean(); }
