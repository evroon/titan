#pragma once

#include "types/Method.h"
#include "types/Callable.h"

class Property : public Callable
{
public:
	Property();
	~Property();

	StringName var_name = "";
	VariantType var_type;

	R_Method_1 *get;
	V_Method_2 *set;
};

struct Variable
{
	Property* property;
	Variant variant;

	Variant get()
	{
		return property->get->operator() (variant);
	}

	void set(const Variant& p_new)
	{
		property->set->operator() (variant, p_new);
	}
};