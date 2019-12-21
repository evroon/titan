#pragma once

#include "resources/Resource.h"
#include "core/Vector.h"
#include "core/Data.h"
#include "Lexer.h"
#include "Parser.h"
#include "Executer.h"
#include "utility/StringUtils.h"
#include "core/NodeManager.h"
#include "resources/TextFile.h"

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