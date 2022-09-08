#include "eventhandler.h"

#if 0

EventHandler::EventHandler(const ObjectType &t, TitanScript *sc) : EventHandler(t)
{
	script = sc;
}

void EventHandler::Init()
{
	InitHandlers(fs);
}

void EventHandler::handle_event(Event *event)
{
	//Run OnEvent
	if (hasMainEventHandler)
		script->RunFunction("OnEvent", Variant(dynamic_cast<Object*>(event)));

	//Run seperate events
	string name = event->get_name();

	if (handlers.contains(name))
		Handle(name, event);
}

void EventHandler::InitHandlers(const Array<string> fs)
{
	if (!script->FunctionExists("OnEvent"))
		hasMainEventHandler = false;

	for (int c = 0; c < fs.size(); c++)
		if (script->FunctionExists(fs[c]))
			handlers.push_back(fs[c]);
}
void EventHandler::Handle(const string &name, Event *e)
{
	script->RunFunction(name, Variant(dynamic_cast<Object*>(e)));
}

void EventHandler::AddCustomEvent(Event *e)
{
	CustomEvents.push_back(e);
}

//SceneEventHandler
void SceneEventHandler::Init()
{
	string names[] = { "Update", "FingerPress", "FingerMove", "MousePress", "MouseMove", "KeyPress" };

	fs.buildarray(names, 5);
	InitHandlers(fs);
}

//WorldObjectEventHandler
void WorldObjectEventHandler::Init()
{
	string names[] = { "Update", "FingerPress", "FingerMove", "MousePress", "MouseMove", "KeyPress", "OnCollide" };

	fs.buildarray(names, 6);
	InitHandlers(fs);
}
void WorldObjectEventHandler::Handle(const string &name, Event *e)
{
	//Only Handle events that overlap the bounding box of the object
	if (name == "MousePress" || name == "MouseMove" || "FingerPress" || name == "FingerMove")
	{
		vec2 i = dynamic_cast<InputEvent*>(e)->pos;
		if (object->CheckOverlap(vec2(i.x, i.y)))
			script->RunFunction(name, Variant(dynamic_cast<Object*>(e)));
	}
	else
		script->RunFunction(name, Variant(dynamic_cast<Object*>(e)));
}

#endif
