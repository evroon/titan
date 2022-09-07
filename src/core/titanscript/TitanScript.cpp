#include "titanscript.h"

#include "core/contentmanager.h"

TitanScript::TitanScript()
{
	state = new State;
	textfile = nullptr;
	lexer = nullptr;
	parser = nullptr;
	exe = nullptr;
}

TitanScript::TitanScript(const String& p_file_name) : TitanScript()
{
	open_file(p_file_name);
}

void TitanScript::open_file(const String& filepath)
{
	set_file(filepath);

	textfile = CONTENT->LoadTextFile(filepath);

	lexer = new Lexer(textfile->get_source());
	parser = new Parser(state, lexer->root);
	exe = new Executer(lexer->root, state);
}

TitanScript* TitanScript::CreateNewInstance()
{
	TitanScript *newscript = new TitanScript;
	Variant ext = state->extension;

	newscript->lexer = lexer;
	newscript->parser = parser;
	newscript->textfile = textfile;
	newscript->exe = exe;

	newscript->state = new State;

	Extend(ext);
	return newscript;
}

void TitanScript::Extend(Variant ext)
{
	state->extension = ext;
	state->extensiontype = ext.o->get_type();
}

bool TitanScript::FunctionExists(const StringName& name)
{
	if (!exe || !exe->state) {
		T_ERROR("exe is null for function: " + name);
		return false;
	}

	return exe->state->FuncExists(name);
}

Variant TitanScript::RunFunction(const StringName& name)
{
	return exe->run_titan_func(name, Array<Variant>());
}

Variant TitanScript::RunFunction(const StringName& name, const Array<Variant>& paras)
{
	return exe->run_titan_func(name, paras);
}

void TitanScript::Clean()
{
	lexer->Free();
	state->Free();

	delete exe;
	delete lexer;
	delete parser;
	delete state;
}

#undef CLASSNAME
#define CLASSNAME TitanScript

void TitanScript::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(String);

	REG_METHOD(open_file);
}
