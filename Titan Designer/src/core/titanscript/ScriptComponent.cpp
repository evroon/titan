#include "ScriptComponent.h"

#include "input/EventHandler.h"

ScriptComponent::ScriptComponent(const String &name)
{
	script = ScriptManager::GetScript(name);

	bind_parent(parent);
	LoadScript();

	//eh = new WorldObjectEventHandler(script);
	//((WorldObjectEventHandler*)eh)->object = (WorldObject*)parent;
}

ScriptComponent::~ScriptComponent()
{
	delete script;
}

void ScriptComponent::bind_parent(WorldObject *pa)
{
	Component::bind_parent(pa);

	script->Extend(parent);
}

void ScriptComponent::LoadScript()
{
	script->open_file(scriptname);
}

void ScriptComponent::handle_event(Event *e)
{
	//((EventHandler*)eh)->handle_event(e);
}