#include "ScriptApp.h"

#include "core/ContentManager.h"
#include "TitanScript.h"

ScriptApp::ScriptApp(Platform *p_platform) : Application(p_platform)
{
	script = nullptr;
}

Variant ScriptApp::execute(const Array<String>& p_args)
{
	if (p_args.size() < 2)
		return NULL_VAR;

	InitEngine();

	script = CONTENT->Load(p_args[0])->cast_to_type<TitanScript*>();
	if (script && script->FunctionExists(p_args[1]))
		return script->RunFunction(p_args[1]);
	
	delete script;
	return NULL_VAR;
}