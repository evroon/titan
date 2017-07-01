#include "PropertyView.h"

#include "VectorField.h"
#include "CheckBox.h"
#include "ResourceField.h"

#include "types/MethodMaster.h"

PropertyView::PropertyView(const Variant &p_var)
{
	set_property(p_var);

	background_color = TO_RGB(40);

	split_percentage = 0.5f;

	margin[0] = 6.0f;
	margin[1] = 6.0f;
	margin[2] = 6.0f;
}

PropertyView::PropertyView() : PropertyView(NULL_VAR)
{

}

PropertyView::~PropertyView()
{

}

vec2 PropertyView::get_required_size() const
{
	return vec2(150);
}

void PropertyView::handle_event(UIEvent * ui_event)
{
	int relevant_item = get_item(ui_event->pos);

}

void PropertyView::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:
		
		draw_box(area, background_color);

		for (int c = 0; c < items.size(); c++)
		{
			vec2 pos = vec2(items[c].area.get_left(), items[c].area.pos.y);
			rect2 area = items[c].area;

			draw_box(area, c % 2 ? TO_RGB(50) : TO_RGB(60));
			draw_text(items[c].name, pos);

			items[c].control->draw();
		}

		if(items.size() > 0)
			draw_line(vec2(split_pos, area.get_top()), vec2(split_pos, area.get_bottom()), TO_RGB(150));
		else
		{
			float width = get_default_font()->get_width("No content to display");

			draw_text("No content to display", area.pos - vec2(width / 2.0f, 0));
		}


		break;

	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		position_items();

		break;
	}
}

void PropertyView::set_property(Object* p_var)
{
	if (p_var == var)
		return;

	var = p_var;

	items.clear();
	clean();

	if (p_var != NULL)
	{
		Array<StringName> names = MMASTER->list_property_names(p_var->get_type());

		for (StringName &n : names)
			add_property(MMASTER->get_property(p_var->get_type(), n));
	}

	position_items();
}

Object* PropertyView::get_property() const
{
	return var;
}

void PropertyView::add_property(Property* p_property)
{
	PropertyItem item;
	item.property = p_property;
	item.var = p_property->get->operator()(var);
	item.name = p_property->var_name;

	Variable variable = { item.property, var };

	int type = item.var.type;

	switch (type)
	{
	case Variant::UNDEF:

		item.control = new Label("NULL");
		item.control->cast_to_type<Label*>()->set_color(TO_RGB(vec3i(214, 45, 36)));

		break;

	case Variant::BOOL:

		item.control = new CheckBox(variable);

		break;

	case Variant::INT:

		item.control = new NumberField(variable);

		break;

	case Variant::FLOAT:

		item.control = new TextField(variable);

		break;

	case Variant::VEC2:

		item.control = new Vec2Field(variable);

		break;

	case Variant::VEC3:

		item.control = new Vec3Field(variable);

		break;

	case Variant::VEC4: //unimplemented

		item.control = new Label("NULL");
		item.control->cast_to_type<Label*>()->set_color(TO_RGB(vec3i(214, 45, 36)));
		break;

	case Variant::COLOR: //unimplemented

		item.control = new Label("NULL");
		item.control->cast_to_type<Label*>()->set_color(TO_RGB(vec3i(214, 45, 36)));
		//item.control = new ColorField(var);

		break;

	case Variant::OBJECT:
		
		if (item.var.o->is_of_type<Texture2D>())
		{
			item.control = new Texture2DField(item.var.o->cast_to_type<Texture2D*>());
			break;
		}

		item.control = new Label(item.var.o->get_type_name());
		item.control->cast_to_type<Label*>()->set_color(TO_RGB(vec3i(116, 198, 105)));

		break;

	case Variant::TRANSFORM:

		item.control = new TransformField(variable);

		break;

	default:

		item.control = new TextField(variable);

		break;
	}

	items.push_back(item);

	add_child(item.control);
	
	position_items();
}

int PropertyView::get_item(const vec2 & p_pos) const
{
	for (int c = 0; c < items.size(); c++)
	{
		if (items[c].area.is_in_box(p_pos))
			return c;
	}
	return -1;
}

void PropertyView::position_items()
{
	float offset_y = 0;

	split_pos = area.get_left() + area.size.x * 2.0f * split_percentage;

	for (int c = 0; c < items.size(); c++)
	{
		float height = items[c].control->get_required_size().y + 8;

		float left = area.get_left() + margin[0];
		float right = area.get_right();
		float top = -offset_y + area.get_top();
		float bottom = -offset_y - height + area.get_top();

		items[c].area = rect2(left, right, top, bottom);
		items[c].control->set_anchors(ANCHOR_CENTER, ANCHOR_END, ANCHOR_END, ANCHOR_END);
		items[c].control->set_margins(4, offset_y + height - 4, 4, offset_y + 4);

		offset_y += height;
	}
}