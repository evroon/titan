#pragma once

#include "Control.h"
#include "Container.h"
#include "core/variant/Variant.h"
#include "Label.h"
#include "TextField.h"

#include "core/Property.h"


class PropertyView : public Control
{
	OBJ_DEFINITION(PropertyView, Control);

public:
	struct PropertyItem
	{
		String name;
		Variant var;
		Property* property;
		Control* control;
		rect2 area;
	};

	PropertyView(const Variant &p_var);
	PropertyView();
	~PropertyView();

	vec2 get_required_size() const;

	void handle_event(UIEvent *ui_event) override;

	void notification(int p_notification) override;

	void set_property(Object* p_var);
	Object* get_property() const;

private:
	void add_property(Property* p_property);

	int get_item(const vec2& p_pos) const;

	void position_items();

	Array<PropertyItem> items;

	Object* var;

	float split_pos;
	float split_percentage;

	float margin[3];

	Color background_color;
};

