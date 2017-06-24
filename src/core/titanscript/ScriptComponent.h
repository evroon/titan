#pragma once

#include "TitanScript.h"
#include "core/variant/Variant.h"

#include "core/ScriptManager.h"

#include "core/Component.h"

#include "input/Event.h"

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