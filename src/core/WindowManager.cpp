#include "WindowManager.h"

WindowManager WindowManager::window_manager;

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
	
}

WindowManager* WindowManager::get_window_manager()
{
	return &window_manager;
}

void WindowManager::register_window(Window *window)
{
	default_window = window;
	windows.push_back(window);
}

Window* WindowManager::get_default_window()
{
	return default_window;
}

vec2i WindowManager::get_default_window_size()
{
	return get_default_window()->size;
}

void WindowManager::clean()
{
	windows.clean();
}