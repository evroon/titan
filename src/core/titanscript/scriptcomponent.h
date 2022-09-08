#pragma once

#include "titanscript.h"
#include "core/variant/variant.h"

#include "core/scriptmanager.h"

#include "core/component.h"

#include "input/event.h"

class ScriptComponent : public Component
{
	OBJ_DEFINITION(ScriptComponent, Component)

public:
	ScriptComponent(const String &name);
	virtual ~ScriptComponent();

	void bind_parent(WorldObject *pa) override;
	void handle_event(Event *e);

	void LoadScript();

	String scriptname;
	TitanScript *script;
	Object *eh;
};
