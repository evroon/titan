#include "methodbuilder.h"

#include "methodmaster.h"

//void method
V_Method_1* MethodBuilder::reg_method(std::function<void(VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	V_Method_1 *result = new V_Method_1;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}
V_Method_2* MethodBuilder::reg_method(std::function<void(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	V_Method_2 *result = new V_Method_2;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}
V_Method_3* MethodBuilder::reg_method(std::function<void(VAR, VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	V_Method_3 *result = new V_Method_3;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}

//return method
R_Method_1* MethodBuilder::reg_method(std::function<VAR(VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	R_Method_1 *result = new R_Method_1;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}
R_Method_2* MethodBuilder::reg_method(std::function<VAR(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	R_Method_2 *result = new R_Method_2;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}
R_Method_3* MethodBuilder::reg_method(std::function<VAR(VAR, VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE)
{
	R_Method_3 *result = new R_Method_3;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	result->inherits_from = var_type;
	MMASTER->register_method(var_type, result);
	return result;
}

//static void func
V_Method_0* MethodBuilder::reg_static_func(std::function<void()> p_func, const StringName &name, const ParameterNames &p_args)
{
	V_Method_0* result = new V_Method_0;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}
V_Method_1* MethodBuilder::reg_static_func(std::function<void(VAR)> p_func, const StringName &name, const ParameterNames &p_args)
{
	V_Method_1* result = new V_Method_1;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}
V_Method_2* MethodBuilder::reg_static_func(std::function<void(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args)
{
	V_Method_2* result = new V_Method_2;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}

//static void func
R_Method_0* MethodBuilder::reg_static_func(std::function<VAR()> p_func, const StringName &name, const ParameterNames &p_args)
{
	R_Method_0* result = new R_Method_0;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}
R_Method_1* MethodBuilder::reg_static_func(std::function<VAR(VAR)> p_func, const StringName &name, const ParameterNames &p_args)
{
	R_Method_1* result = new R_Method_1;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}
R_Method_2* MethodBuilder::reg_static_func(std::function<VAR(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args)
{
	R_Method_2* result = new R_Method_2;
	result->param_names = p_args;
	result->param_types = MethodBinder::get_singleton()->arg_typenames;
	result->is_const = MethodBinder::get_singleton()->is_const;
	result->return_type = VariantType(MethodBinder::get_singleton()->return_type);
	result->func = p_func;
	result->name = name;
	MMASTER->register_static_func(name, result);
	return result;
}

//constructor
CSTR_0* MethodBuilder::register_constructor(std::function<VAR()> p_func, const ParameterNames &p_args, VAR_TYPE)
{
	CSTR_0 *result = new CSTR_0;
	result->params = p_args;
	result->func = p_func;
	MMASTER->register_constructor(var_type, result);
	return result;
}
CSTR_1* MethodBuilder::register_constructor(std::function<VAR(VAR)> p_func, const ParameterNames &p_args, VAR_TYPE)
{
	CSTR_1 *result = new CSTR_1;
	result->params = p_args;
	result->func = p_func;
	MMASTER->register_constructor(var_type, result);
	return result;
}
CSTR_2* MethodBuilder::register_constructor(std::function<VAR(VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE)
{
	CSTR_2 *result = new CSTR_2;
	result->params = p_args;
	result->func = p_func;
	MMASTER->register_constructor(var_type, result);
	return result;
}
CSTR_3* MethodBuilder::register_constructor(std::function<VAR(VAR, VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE)
{
	CSTR_3 *result = new CSTR_3;
	result->params = p_args;
	result->func = p_func;
	MMASTER->register_constructor(var_type, result);
	return result;
}
CSTR_4* MethodBuilder::register_constructor(std::function<VAR(VAR, VAR, VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE)
{
	CSTR_4 *result = new CSTR_4;
	result->params = p_args;
	result->func = p_func;
	MMASTER->register_constructor(var_type, result);
	return result;
}

// Property
Property* MethodBuilder::register_property(R_Method_1 *p_get, V_Method_2 *p_set, const StringName &name, VAR_TYPE)
{
	Property *result = new Property;
	result->get = p_get;
	result->set = p_set;
	result->var_name = name;
	result->var_type = result->get->return_type;
	result->inherits_from = var_type;
	MMASTER->register_property(var_type, result);
	return result;
}