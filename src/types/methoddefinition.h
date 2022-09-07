#pragma once

#include "core/string.h"
#include "core/array.h"

#define str const String &

class MethodDefinition
{
public:
	MethodDefinition(str p_call_name);
	MethodDefinition(str p_call_name, str p_arg_0);
	MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1);
	MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2);
	MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2, str p_arg_3);
	MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2, str p_arg_3, str p_arg_4);

	int get_arg_count();

	String call_name;
	Array<std::string> arg_names;
};
