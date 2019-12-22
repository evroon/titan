#pragma once

#include "core/Application.h"

class ScriptApp : public Application
{
	OBJ_DEFINITION(ScriptApp, Application);

public:
	ScriptApp(Platform *p_platform);

	Variant execute(const Array<String>& p_args);
	
private:
	TitanScript* script;
};