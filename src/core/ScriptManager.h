#pragma once

#include "core/titanscript/TitanScript.h"
#include "Vector.h"

class ScriptManager
{
public:
	static TitanScript* GetScript(const String &name) { return nullptr; }

private:
	static TitanScript* AddScript(const String &name) { return nullptr; }

	static Vector<TitanScript> scripts;
};

