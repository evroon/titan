#include "PropertyView.h"

#include "VectorField.h"
#include "CheckBox.h"
#include "ResourceField.h"
#include "ImageButton.h"

#include "types/MethodMaster.h"

PropertyView::PropertyView(const Variant &p_var)
{
	roots = Array<GroupItem>();
	buttons = Container();
	set_property(p_var);

	background_color = TO_RGB(40);

	split_percentage = 0.5f;

	margin[0] = 6.0f;
	margin[1] = 6.0f;
	margin[2] = 6.0f;

	back = nullptr;
	forward = nullptr;
	var = nullptr;

	split_pos = 0.0f;
	offset = 0.0f;
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

void PropertyView::handle_event(UIEvent* ui_event)
{
	int relevant_item = get_item(ui_event->pos);
}

void PropertyView::draw_item(const PropertyItem& p_item)
{
	vec2 pos = vec2(p_item.area.get_left(), p_item.area.pos.y);
	rect2 area = p_item.area;

	draw_text(p_item.name, pos);

	//p_item.control->draw();
}

void PropertyView::draw_item(const GroupItem& p_item)
{
	vec2 pos = vec2(p_item.area.pos.x - get_default_font()->get_width(p_item.name) / 2.0f, p_item.area.pos.y);

	draw_box(p_item.area, TO_RGB(100));
	draw_text(p_item.name, pos);

	for (int c = 0; c < p_item.children.size(); c++)
	{
		draw_item(p_item.children[c]);
	}
}

void PropertyView::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:
		draw_box(area, background_color);

		for (int c = 0; c < roots.size(); c++)
		{
			for (int i = 0; i < roots[c].children.size(); i++)
			{
				PropertyControl* pc = dynamic_cast<PropertyControl*>(roots[c].children[i].control);
				if (pc && !pc->get_focused())
					pc->update_value();
			}

			draw_item(roots[c]);
		}

		if (roots.size() > 0)
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

	roots.clear();
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
	case Variant::BOOL:

		item.control = new CheckBox(variable);
		break;

	case Variant::INT:

		item.control = new NumberField(variable);
		break;

	case Variant::FLOAT:

		item.control = new NumberField(variable);
		break;

	case Variant::VEC2:

		item.control = new Vec2Field(variable);
		break;

	case Variant::VEC3:

		item.control = new Vec3Field(variable);
		break;

	case Variant::VEC4:

		item.control = new Vec4Field(variable);
		break;

	case Variant::COLOR:

		item.control = new ColorField(variable);
		break;

	case Variant::UNDEF:
	case Variant::OBJECT:
				
		if (item.property->var_type.is_of_type<Texture2D>())
		{
			item.control = new Texture2DField(variable);
			break;
		}

		item.control = new ObjectField(variable);
		break;

	case Variant::TRANSFORM:

		item.control = new TransformField(variable);
		break;

	case Variant::STRING:

		item.control = new TextField(variable);
		break;

	case Variant::ARRAY:

		item.control = new TextField(variable);
		break;

	default:

		item.control = new TextField(variable);
		break;
	}

	String inherits = p_property->inherits_from.get_type_name();
	bool found = false;

	for (int c = 0; c < roots.size(); c++)
	{
		if (roots[c].name == inherits)
		{
			roots[c].children.push_back(item);
			found = true;
		}
	}

	if (!found)
	{
		roots.push_back({ inherits, rect2(), item });
	}

	add_child(item.control);
	
	position_items();
}

int PropertyView::get_item(const vec2& p_pos) const
{
	for (int c = 0; c < roots.size(); c++)
	{
		//if (items[c].area.is_in_box(p_pos))
		//	return c;
	}
	return -1;
}

void PropertyView::position_item(GroupItem& p_item)
{
	float left = area.get_left() + margin[0];
	float right = area.get_right() - margin[1];
	float top = -offset + area.get_top();
	float bottom = -offset - 20 + area.get_top();

	p_item.area = rect2(left, right, top, bottom);
	offset += 20;

	for (int c = 0; c < p_item.children.size(); c++)
	{
		PropertyItem& p = p_item.children[c];
		float height = p.control->get_required_size().y + 8;

		float left = area.get_left() + margin[0];
		float right = area.get_right() - margin[1];
		float top = -offset + area.get_top();
		float bottom = -offset - height + area.get_top();

		p.area = rect2(left, right, top, bottom);
		p.control->set_anchors(ANCHOR_CENTER, ANCHOR_END, ANCHOR_END, ANCHOR_END);
		p.control->set_margins(4, offset + height - 4, 6, offset + 4);

		offset += height;
	}
}

void PropertyView::position_items()
{
	offset = 4;
	split_pos = area.get_left() + area.size.x * 2.0f * split_percentage;

	for (int c = 0; c < roots.size(); c++)
	{
		position_item(roots[c]);
	}
	update();
}

#undef CLASSNAME
#define CLASSNAME PropertyView

void PropertyView::bind_methods()
{
	REG_PROPERTY(property);
}