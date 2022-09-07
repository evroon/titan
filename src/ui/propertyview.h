#pragma once

#include "control.h"
#include "container.h"
#include "core/variant/variant.h"
#include "label.h"
#include "textfield.h"
#include "imagebutton.h"

#include "core/property.h"


class PropertyView : public Control
{
	OBJ_DEFINITION(PropertyView, Control);

public:
	struct PropertyItem
	{
		String name = "";
		Variant var = NULL_VAR;
		Property* property = nullptr;
		Control* control = nullptr;
		rect2 area = rect2();
	};
	struct GroupItem
	{
		String name = "";
		rect2 area = rect2();
		Array<PropertyItem> children = {};
	};

	PropertyView(const Variant &p_var);
	PropertyView();
	~PropertyView();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void draw_item(const PropertyItem& p_item);
	void draw_item(const GroupItem& p_item);

	void notification(int p_notification) override;

	void set_property(Object* p_var);
	Object* get_property() const;

	static void bind_methods();

private:
	void add_property(Property* p_property);

	int get_item(const vec2& p_pos) const;

	void position_item(GroupItem& p_item);
	void position_items();

	Array<GroupItem> roots;

	Container buttons;

	ImageButton* back;
	ImageButton* forward;

	Object* var;

	float split_pos;
	float split_percentage;

	float margin[3];

	Color background_color;

	float offset;
};
