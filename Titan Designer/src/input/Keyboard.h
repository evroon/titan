#pragma once

#include "Key.h"

#define KEYBOARD Keyboard::get_singleton()


class Keyboard : public Object
{
	OBJ_DEFINITION(Keyboard, Object);

public:
	Keyboard();
	~Keyboard();

	bool is_button_pressed(Key::KeyType p_key);

	static void init();
	static Keyboard* get_singleton();
	static void bind_methods();

private:
	static Keyboard *singleton;
};

