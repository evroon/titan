#pragma once

#include "core/data.h"
#include "scriptnode.h"
#include "core/tmessage.h"

#define PARSE_ERROR(X) report_error(ParseError(X, __FILE__, __LINE__));
#define PARSE_WARNING(X) report_warning(ParseWarning(X, __FILE__, __LINE__));

struct ParseError : public TError
{
	ParseError(const String &p_description, const String &p_file_name, int p_line_number) : TError(p_description, p_file_name, p_line_number) { }
};

struct ParseWarning : public TWarning
{
	ParseWarning(const String &p_description, const String &p_file_name, int p_line_number) : TWarning(p_description, p_file_name, p_line_number) { }
};

class Parser
{
public:
	Parser(State *_state, Line &line);
	void Parse(Line &line);
	int GetFirstIndex(const Array<Token> &tokens, const String src[], int srccount);
	Composition* GetComposition(const Line &line);
	Block* ParseBlock(const Line &line);

	//Parse recursively and obey operator precedence
	ScriptNode* ParsePart(const Line &line);
	ScriptNode *ParsePath(const Line &line);
	ScriptNode *ParseMember(const Line &line);
	ScriptNode *ParseMemberVar(const Line &line);
	ScriptNode *ParseMemberFunc(const Line &line);

	//error handling
	void report_error(const ParseError &err);
	void report_warning(const ParseWarning &warn);

private:

	struct Definition
	{
		StringName name;
		Variant value;
	};

	Definition* get_definition(const StringName &p_name);

	Line *parent;
	State *state;

	Array<ParseWarning> warnings;
	Array<ParseError> errors;

	Array<Definition> definitions;

	int subindex = 0;

	//used for Path:
	VariantType parent_type;
};