#pragma once

#include "core/Application.h"

class ScriptApp : public Application
{
	OBJ_DEFINITION(ScriptApp, Application);

public:
	ScriptApp(Platform *p_platform);

	void execute(const Array<String>& p_args);
	
private:
	TitanScript* script;
};