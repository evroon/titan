#include "keyboard.h"

Keyboard* Keyboard::singleton;

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

bool Keyboard::is_button_pressed(Key::KeyType p_key)
{
	return SDL_GetKeyboardState(NULL)[Key(p_key).get_SDL_Scancode()] != 0;
}

bool Keyboard::is_key_pressed(Key* p_key)
{
	return SDL_GetKeyboardState(NULL)[p_key->get_SDL_Scancode()] != 0;
}

void Keyboard::init()
{
	singleton = new Keyboard;
}

Keyboard* Keyboard::get_singleton()
{
	return singleton;
}

#undef CLASSNAME
#define CLASSNAME Keyboard

void Keyboard::bind_methods()
{
	REG_SINGLETON(KEYBOARD);

	REG_METHOD(is_key_pressed);
}