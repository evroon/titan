#pragma once

#include "Control.h"

class PropertyControl : public Control
{
	OBJ_DEFINITION(PropertyControl, Control);

public:
	PropertyControl();
	PropertyControl(const Variant& p_variant);
	PropertyControl(const Variable& p_variable);

	virtual ~PropertyControl();

	void update_value();
	virtual void value_changed() { }

	virtual void value_input() { }

	void set_value(const Variant& p_value);
	void set_value(const Variable& p_variable);

	Variant get_value();

	static void bind_methods();

private:
	union
	{
		Variant variant;
		Variable variable;
	};

	enum Type
	{
		VARIANT,
		VARIABLE
	} type;
};

