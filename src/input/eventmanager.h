#pragma once

#include "event.h"

class EventManager
{
public:
	EventManager();
	~EventManager();

	void register_event(Event *e);
	void clear_event(Event *e);

	void clean();

	static void init();
	static EventManager& get_manager();

private:
	static EventManager *default_manager;

	Vector<Event> events;
};
