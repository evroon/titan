#include "PropertyControl.h"



PropertyControl::PropertyControl()
{
	set_value(Variant());
}

PropertyControl::PropertyControl(const Variant& p_value)
{
	set_value(p_value);
}

PropertyControl::PropertyControl(const Variable& p_variable)
{
	set_value(p_variable);
}

PropertyControl::~PropertyControl()
{
}

void PropertyControl::update_value()
{
	if (type == VARIABLE)
		set_value(variable.get());

	value_changed();
}

void PropertyControl::set_value(const Variant& p_value)
{
	type = VARIANT;
	variant = p_value;

	value_changed();
}

void PropertyControl::set_value(const Variable& p_variable)
{
	type = VARIABLE;
	variable = p_variable;

	value_changed();
}

Variant PropertyControl::get_value()
{
	if (type == VARIANT)
	{
		if (variant.isdef())
			return variant;
		
		return Variant();
	}

	return variable.get();
}

#undef CLASSNAME
#define CLASSNAME PropertyControl

void PropertyControl::bind_methods()
{
	REG_CSTR_OVRLD_1(Variant);

	REG_METHOD(value_changed);

	REG_SIGNAL("value_changed");
}
