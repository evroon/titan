#pragma once

#include "Tab.h"
#include "PropertyView.h"

class PropertyTab : public Tab
{
	OBJ_DEFINITION(PropertyTab, Tab);

public:
	PropertyTab(Object* p_object);
	PropertyTab();

	virtual ~PropertyTab();

	vec2 get_required_size() const;

	void set_property(Object* p_object);
	Object* get_property() const;

	static void bind_methods();

private:
	PropertyView *property_view;
	TextField* name_field;
	

	float buttons_height = 30.0f;
	rect2 bar_area, work_area;

	Container bar;
};

