#include "methoddefinition.h"

#define OVERLOAD_CSTR_0 : MethodDefinition::MethodDefinition(p_call_name)
#define OVERLOAD_CSTR_1 : MethodDefinition::MethodDefinition(p_call_name, p_arg_0)
#define OVERLOAD_CSTR_2 : MethodDefinition::MethodDefinition(p_call_name, p_arg_0, p_arg_1)
#define OVERLOAD_CSTR_3 : MethodDefinition::MethodDefinition(p_call_name, p_arg_0, p_arg_1, p_arg_2)
#define OVERLOAD_CSTR_4 : MethodDefinition::MethodDefinition(p_call_name, p_arg_0, p_arg_1, p_arg_2, p_arg_3)
#define OVERLOAD_CSTR_5 : MethodDefinition::MethodDefinition(p_call_name, p_arg_0, p_arg_1, p_arg_2, p_arg_3, p_arg_4)

MethodDefinition::MethodDefinition(str p_call_name)
{
	call_name = p_call_name;
}

MethodDefinition::MethodDefinition(str p_call_name, str p_arg_0) OVERLOAD_CSTR_0
{
	arg_names.push_back(p_arg_0);
}

MethodDefinition::MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1) OVERLOAD_CSTR_1
{
	arg_names.push_back(p_arg_1);
}

MethodDefinition::MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2) OVERLOAD_CSTR_2
{
	arg_names.push_back(p_arg_2);
}

MethodDefinition::MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2, str p_arg_3) OVERLOAD_CSTR_3
{
	arg_names.push_back(p_arg_3);
}

MethodDefinition::MethodDefinition(str p_call_name, str p_arg_0, str p_arg_1, str p_arg_2, str p_arg_3, str p_arg_4) OVERLOAD_CSTR_4
{
	arg_names.push_back(p_arg_4);
}

int MethodDefinition::get_arg_count()
{
	return arg_names.size();
}