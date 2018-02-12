#pragma once

#include <iostream>

#include "core/Data.h"
#include "TsVariable.h"

class Executer
{
public:
	Executer();
	Executer(Line line, State *state);

	~Executer() { delete state; }

	Variant run_method(Method * m, Array<Variant>& args);

	Variant GetMemberMinusOne(const Path &var);
	Variant GetMember(const Path &var);

	Property* get_property(const Path &var);

	void SetVariable(ScriptNode *node, Variant val);

	Variant run_member_func(Variant &object, MemberFunc *mf);
	Variant run_titan_func(const String &name, Array<Variant> paras);

	Variant Execute(ScriptNode *node);

	State *state;
	Block *activefunc;

	bool returntofunc;
};

struct SimpleExecuter
{
	static Variant execute(ScriptNode *p_node)
	{
		Executer exe;
		return exe.Execute(p_node);
	}
};