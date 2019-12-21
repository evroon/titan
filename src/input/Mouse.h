#pragma once

#include "core/Definitions.h"

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "math/Vec2.h"
#include "core/Object.h"

#define MOUSE Mouse::get_singleton()

class Mouse : public Object
{
	OBJ_DEFINITION(Mouse, Object);

public:
	Mouse();

	enum ButtonType
	{
		LEFT,
		CENTER,
		RIGHT
	};

	int get_SDL_type(int type);
	ButtonType get_ButtonType(int SDL_type);
	bool is_pressed(int type);
	vec2 get_position();

	void set_position(const vec2i& p_pos);

	static void init();
	static Mouse* get_singleton();

	static void bind_methods();

private:
	static Mouse *singleton;
};

