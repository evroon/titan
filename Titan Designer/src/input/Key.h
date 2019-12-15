#pragma once

#include <string>

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "core/Object.h"

class Key : public Object
{
	OBJ_DEFINITION(Key, Object)

public:
	enum KeyType
	{
		//All letters
		KEY_A = 'a',
		KEY_B = 'b',
		KEY_C = 'c',
		KEY_D = 'd',
		KEY_E = 'e',
		KEY_F = 'f',
		KEY_G = 'g',
		KEY_H = 'h',
		KEY_I = 'i',
		KEY_J = 'j',
		KEY_K = 'k',
		KEY_L = 'l',
		KEY_M = 'm',
		KEY_N = 'n',
		KEY_O = 'o',
		KEY_P = 'p',
		KEY_Q = 'q',
		KEY_R = 'r',
		KEY_S = 's',
		KEY_T = 't',
		KEY_U = 'u',
		KEY_V = 'v',
		KEY_W = 'w',
		KEY_X = 'x',
		KEY_Y = 'y',
		KEY_Z = 'z',

		//All operators
		KEY_PLUS = SDLK_KP_PLUS,
		KEY_MINUS = SDLK_KP_MINUS,
		KEY_MULTIPLY = SDLK_KP_MULTIPLY,
		KEY_DIVIDE = SDLK_KP_DIVIDE,
		KEY_POWER = SDLK_KP_POWER,

		//All numbers
		KEY_0 = '0',
		KEY_1 = '1',
		KEY_2 = '2',
		KEY_3 = '3',
		KEY_4 = '4',
		KEY_5 = '5',
		KEY_6 = '6',
		KEY_7 = '7',
		KEY_8 = '8',
		KEY_9 = '9',

		//Computer stuff
		KEY_RETURN = '\r',
		KEY_ESCAPE = '\033',
		KEY_BACKSPACE = '\b',
		KEY_TAB = '\t',
		KEY_SPACE = ' ',
		KEY_DOT = '.',

		KEY_QUOTE = '\'',
		KEY_SEMICOLON = ';',
		KEY_SLASH = '/',
		KEY_COMMA = ',',
		KEY_LEFTBRACKET = '[',
		KEY_RIGHTBRACKET = ']',
		KEY_BACKSLASH = '\\',

		KEY_DELETE = SDLK_DELETE,
		KEY_PAGE_UP = SDLK_PAGEUP,
		KEY_PAGE_DOWN = SDLK_PAGEDOWN,
		KEY_HOME = SDLK_HOME,
		KEY_END = SDLK_END,
		KEY_ALT = SDLK_ALTERASE,
		KEY_LCONTROL = SDLK_LCTRL,
		KEY_RCONTROL = SDLK_RCTRL,
		KEY_LSHIFT = SDLK_LSHIFT,
		KEY_RSHIFT = SDLK_RSHIFT,
		KEY_CAPS_LOCK = SDLK_CAPSLOCK,

		KEY_UP = SDLK_UP,
		KEY_DOWN = SDLK_DOWN,
		KEY_LEFT = SDLK_LEFT,
		KEY_RIGHT = SDLK_RIGHT,

		//F-keys
		KEY_F1 = SDLK_F1,
		KEY_F2 = SDLK_F2,
		KEY_F3 = SDLK_F3,
		KEY_F4 = SDLK_F4,
		KEY_F5 = SDLK_F5,
		KEY_F6 = SDLK_F6,
		KEY_F7 = SDLK_F7,
		KEY_F8 = SDLK_F8,
		KEY_F9 = SDLK_F9,
		KEY_F10 = SDLK_F10,
		KEY_F11 = SDLK_F11,
		KEY_F12 = SDLK_F12,
		KEY_F13 = SDLK_F13,

		//Android & iOS
		KEY_BACK = SDLK_AC_BACK,
		KEY_MENU = SDLK_AC_HOME
	};

	Key() { }
	Key(const KeyType &_key_type) : Key() { key_type = _key_type; }
	Key(const SDL_Keycode &key) : Key() { set_key_type(key); }
	Key(const String &p_name) : Key() { set_using_name(p_name); }


	String to_string() const
	{
		String s = SDL_GetKeyName(key_type);
		return s;
	}

	void set_using_name(const String &p_name)
	{
		key_type = SDL_GetKeyFromName(p_name.c_str());
	}

	void set_key_type(SDL_Keycode key)
	{
		key_type = key;
	}

	SDL_Keycode get_SDL_Keycode() const
	{
		return key_type;
	}
	SDL_Scancode get_SDL_Scancode() const
	{
		return SDL_GetScancodeFromKey(get_SDL_Keycode());
	}

	bool operator==(const Key &r)
	{
		return key_type == r.key_type;
	}

	
	int key_type;


#undef CLASSNAME
#define CLASSNAME Key

	static void bind_methods()
	{
		REG_CSTR_OVRLD_1(String);

		REG_METHOD(to_string);

	}
};

class ModKey
{
public:
	enum KeyType
	{
		KEY_MOD_NONE,
		KEY_MOD_NUM,
		KEY_MOD_CAPS,
		KEY_MOD_LCTRL,
		KEY_MOD_RCTRL,
		KEY_MOD_RSHIFT,
		KEY_MOD_LSHIFT,
		KEY_MOD_RALT,
		KEY_MOD_LALT,
		KEY_MOD_CTRL,
		KEY_MOD_SHIFT,
		KEY_MOD_ALT
	};
	
	ModKey() { }
	ModKey(KeyType p_type)
	{
		type = p_type;
	}

#define CASE(X) case K##X: type = KEY_##X; break;

	ModKey(int SDL_type)
	{
		switch (SDL_type)
		{
			CASE(MOD_NONE)
			CASE(MOD_NUM)
			CASE(MOD_CAPS)
			CASE(MOD_LCTRL)
			CASE(MOD_RCTRL)
			CASE(MOD_RSHIFT)
			CASE(MOD_LSHIFT)
			CASE(MOD_RALT)
			CASE(MOD_LALT)
			CASE(MOD_CTRL)
			CASE(MOD_SHIFT)
			CASE(MOD_ALT)

			default:
				type = KEY_MOD_NONE;
		}
	}

	KeyType type;
};