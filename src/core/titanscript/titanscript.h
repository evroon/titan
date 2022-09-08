#pragma once

#include "resources/resource.h"
#include "core/vector.h"
#include "core/data.h"
#include "lexer.h"
#include "parser.h"
#include "executer.h"
#include "utility/stringutils.h"
#include "core/nodemanager.h"
#include "resources/textfile.h"

class TitanScript : public Resource
{
	OBJ_DEFINITION(TitanScript, Resource)

public:
	TitanScript();
	TitanScript(const String &p_file_name);

	void open_file(const String &filepath);

	TitanScript* CreateNewInstance();

	void Extend(Variant ext);

	bool FunctionExists(const StringName &name);

	Variant RunFunction(const StringName &name);
	Variant RunFunction(const StringName &name, const Array<Variant> &paras);

	//Free Memory
	void Clean();

	static void bind_methods();

private:
	TextFile* textfile;
	Lexer *lexer;
	Parser *parser;
	State *state;
	Executer *exe;
};
