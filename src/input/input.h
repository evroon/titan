#pragma once

#include <map>
#include <string>

#include "graphics/renderer.h"

#include "event.h"
#include "key.h"

#define INPUT Input::singleton

class Application;

class Input : public Object
{
	OBJ_DEFINITION(Input, Object);

public:
	Input();

	static void init();

	void HandleEvent(const SDL_Event &event);
	void AddEvent(Event *e);
	void Clean();

	void enable_text_input();
	void disable_text_input();

	void Free();

	static void bind_methods();

    bool fingerdown;
    vec2 mousepos;
	Application *parent;

	Vector<Event> events;

	static Input *singleton;
};