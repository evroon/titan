#include "Slider.h"

#include "ScrollContainer.h"

Slider::Slider()
{
	inner_height = 30;

	space = 0;
}

void Slider::step_down()
{
	set_slider_pos(position + 0.1f);
}

void Slider::step_up()
{
	set_slider_pos(position - 0.1f);
}

float Slider::get_value() const
{
	return position;
}

void Slider::set_slider_pos(float p_position)
{
	float new_position = Math::clip(p_position, 0.0f, 1.0f);

	if (new_position == position)
		return;

	position = new_position;

	emit_signal("value_changed");
}

float Slider::get_slider_pos(const vec2 &pos) const
{
	float top = area.get_top() - space - inner_height;
	float bottom = area.get_bottom() + space + inner_height;
	float pos_y = pos.y;
	float result = -(pos_y - top) / (top - bottom);

	return Math::clip(result, 0.0f, 1.0f);	
}

void Slider::handle_event(UIEvent *ui_event)
{
	switch (ui_event->type)
	{
		case UIEvent::MOUSE_PRESS:

			if (ui_event->press_type == UIEvent::DOWN)
			{
				is_dragging = true;

				if (inner_box.is_in_box(ui_event->pos))
					grab_pos = ui_event->pos;
				else
					set_slider_pos(get_slider_pos(ui_event->pos));
			}
			else
			{
				is_dragging = false;
				grab_pos = vec2();
			}
			break;
			
		case UIEvent::MOUSE_HOVER:

			if (is_dragging)
			{
				set_slider_pos(get_slider_pos(ui_event->pos));
			}
			break;

		case UIEvent::KEY_PRESS:

			if (ui_event->press_type == UIEvent::UP)
				return;

			if (ui_event->key == Key::KEY_DOWN)
				step_down();
			else if (ui_event->key == Key::KEY_UP)
				step_up();

			if (ui_event->key == Key::KEY_LEFT)
				set_slider_pos(0.0);
			else if (ui_event->key == Key::KEY_RIGHT)
				set_slider_pos(1.0);
			break;

		case UIEvent::MOUSE_SCROLL:

			if (ui_event->scroll_type == Event::SCROLL_DOWN)
				step_down();
			else
				step_up();

			break;
	}
}

void Slider::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		set_slider_pos(0.0f);

		break;

	case NOTIFICATION_RESIZED:

		set_slider_pos(0.0f);

		break;

	case NOTIFICATION_DRAW:

		float top = area.get_top() - space - inner_height;
		float bottom = area.get_bottom() + space + inner_height;

		float pos_y = top - (top - bottom) * position;

		inner_box = rect2(area.get_left(), area.get_right(), pos_y + inner_height, pos_y - inner_height);

		draw_box(area, Color::DarkGrey);
		draw_box(inner_box, Color::Grey);

		break;
	}
}

vec2 Slider::get_required_size() const
{
	return vec2(10);
}

#undef CLASSNAME
#define CLASSNAME Slider

void Slider::bind_methods()
{
	REG_CSTR(0);

	REG_SIGNAL("value_changed");
}