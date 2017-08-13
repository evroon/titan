#include "Mouse.h"

#include "core/WindowManager.h"

Mouse *Mouse::singleton;

Mouse::Mouse()
{

}

void Mouse::init()
{
	singleton = new Mouse;
}

Mouse* Mouse::get_singleton()
{
	return singleton;
}


int Mouse::get_SDL_type(int type)
{
	switch (type)
	{
		case LEFT:
			return SDL_BUTTON_LEFT;

		case CENTER:
			return SDL_BUTTON_MIDDLE;

		case RIGHT:
			return SDL_BUTTON_RIGHT;

		default:
			return -1;
	}
}

Mouse::ButtonType Mouse::get_ButtonType(int SDL_type)
{
	switch (SDL_type)
	{
		case SDL_BUTTON_LEFT:
			return LEFT;

		case SDL_BUTTON_MIDDLE:
			return CENTER;

		case SDL_BUTTON_RIGHT:
			return RIGHT;

		default:
			return LEFT;
	}
}

bool Mouse::is_pressed(int type)
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(get_SDL_type(type))) != 0;
}

vec2 Mouse::get_position()
{
	vec2i pos;

	SDL_GetMouseState(&pos.x, &pos.y);

	pos -= WINDOWSIZE / 2;
	pos.y *= -1;

	return vec2(to_float(pos.x), to_float(pos.y));
}

void Mouse::set_position(const vec2i & p_pos)
{
	SDL_WarpMouseInWindow(WINDOW->window, p_pos.x, p_pos.y);
}

#undef CLASSNAME
#define CLASSNAME Mouse

void Mouse::bind_methods()
{
	REG_SINGLETON(singleton);
	REG_METHOD(get_position);
	REG_METHOD(is_pressed);

	REG_CONSTANT(LEFT);
	REG_CONSTANT(CENTER);
	REG_CONSTANT(RIGHT);
}