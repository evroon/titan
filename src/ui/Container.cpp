#include "Container.h"

#include "Viewport.h"
#include "Dock.h"
#include "PropertyView.h"

Container::Container()
{
	is_vert = false;

	left_margin = 4;
	right_margin = 4;
	top_margin = 4;
	bottom_margin = 4;
}


Container::~Container()
{
	clean();
}

void Container::set_glue_vert(bool p_glue_vert)
{
	is_vert = p_glue_vert;
}

void Container::position_childs()
{
	if (!is_vert)
	{
		//childs are placed from left to right
		float top = get_area().get_top() - left_margin;
		float bottom = get_area().get_bottom() + bottom_margin;

		float offset_x = get_area().get_left() + left_margin;

		for (int c = 0; c < children.size(); c++)
		{
			Control* control = children[c]->cast_to_type<Control*>();

			vec2 minimum_size = control->get_required_size();

			rect2 area = rect2(offset_x, offset_x + minimum_size.x, top, bottom);

			control->set_area(area);

			offset_x = area.get_right();
		}
	}
	else
	{
		//childs are placed from top to bottom
		float left = get_area().get_left() + left_margin;
		float right = get_area().get_right() - right_margin;

		float offset_y = get_area().get_top() + top_margin;

		for (int c = 0; c < children.size(); c++)
		{
			Control* control = children[c]->cast_to_type<Control*>();

			vec2 minimum_size = control->get_required_size();

			rect2 area = rect2(left, right, offset_y, offset_y - minimum_size.y);

			control->set_area(area);

			offset_y = area.get_bottom();
		}
	}
}

vec2 Container::get_required_size() const
{
	float height = 0, width = 0.0f;

	for (int c = 0; c < children.size(); c++)
	{
		Control* control = children[c]->cast_to_type<Control*>();
		vec2 req_size = control->get_required_size();

		if (is_vert)
		{
			height += req_size.y;

			if (width < req_size.x)
				width = req_size.x;
		}
		else
		{
			width += req_size.x;

			if (height < req_size.y)
				height = req_size.y;
		}
	}

	width = MAX(10, width);
	height = MAX(10, height);

	return vec2(width + left_margin + right_margin, height + top_margin + bottom_margin);
}

void Container::notification(int p_notification)
{
	switch (p_notification)
	{

	case NOTIFICATION_READY:



		break;

	case NOTIFICATION_DRAW:


		break;

	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		position_childs();

		break;
	}
}

void Container::add_child(Control *p_child)
{
	Node::add_child(p_child);
	p_child->bind_parent(this);

	p_child->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_BEGIN);

	size_changed();

	position_childs();
}

void Container::remove_child(Control *_child)
{
	children.clean(_child);
}

void Container::clean()
{
	children.clean();
}

int Container::get_child_count() const
{
	return children.size();
}
