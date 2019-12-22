#include "ScriptApp.h"

#include "core/ContentManager.h"
#include "TitanScript.h"

ScriptApp::ScriptApp(Platform *p_platform) : Application(p_platform)
{
	script = nullptr;
}

void ScriptApp::execute(const Array<String>& p_args)
{
	if (p_args.size() < 1)
		return;

	InitEngine();

	script = CONTENT->Load(p_args[0])->cast_to_type<TitanScript*>();
	if (script && script->FunctionExists("main")) {
		T_LOG("Executing main...");
		T_LOG(script->RunFunction("main"));
	}
}