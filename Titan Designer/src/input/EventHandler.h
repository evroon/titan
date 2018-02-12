#pragma once

#include "Input.h"

#include "core/titanscript/TitanScript.h"

#if 0

class EventHandler : public Object
{
public:
	EventHandler() { }
	EventHandler(const ObjectType &t, TitanScript *sc);
	virtual ~EventHandler() { }

	//methods
	virtual void Init();
	virtual void handle_event(Event *event);
	virtual void Handle(const string &name, Event *e);

	void AddCustomEvent(Event *e);
	
	//Binded script
	TitanScript *script;
	
	//Has OnEvent func
	bool hasMainEventHandler = true;

	//All names of the active event handlers
	Array<string> handlers;

	//All handler names
	Array<string> fs;

	Vector<Event> CustomEvents;

protected:
	void InitHandlers(const Array<string> fs);
};

class SceneEventHandler : public EventHandler
{
public:
	SceneEventHandler(TitanScript *sc) : EventHandler(ObjectType::SCENEEVENTHANDLER) { script = sc; }
	virtual ~SceneEventHandler() { }

	void Init() override;
};

class WorldObjectEventHandler : public EventHandler
{
public:
	virtual ~WorldObjectEventHandler() { }

	void Init() override;
	void Handle(const string &name, Event *e) override;

	WorldObject *object;
};

#endif