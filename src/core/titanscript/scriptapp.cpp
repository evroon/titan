#include "scriptapp.h"

#include "core/contentmanager.h"
#include "titanscript.h"

ScriptApp::ScriptApp(Platform *p_platform) : Application(p_platform)
{
	script = nullptr;
	graphics_enabled = false;
}

Variant ScriptApp::execute(const Array<String>& p_args)
{
	if (p_args.size() < 2)
		return NULL_VAR;

	InitEngine();

	Variant result;

	script = new TitanScript(File(p_args[0]).get_absolute_path());
	if (script && script->FunctionExists(p_args[1]))
		result = script->RunFunction(p_args[1]);

	delete script;
	return result;
}