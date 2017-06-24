#include "Button.h"

Button::Button()
{

}

void Button::handle_event(UIEvent *ui_event)
{
	switch (ui_event->type)
	{
		case UIEvent::MOUSE_ENTER:

			start_highlight();

			break;

		case UIEvent::MOUSE_EXIT:

			end_highlight();

			break;

		case UIEvent::MOUSE_PRESS:

			if (ui_event->button_type != Mouse::LEFT)
				return;

			if (ui_event->press_type == Event::DOWN)
				start_selection();
			else
			{
				end_selection();

				if (!in_area(ui_event->pos))
					end_highlight();
				else
					emit_signal("clicked");
			}

			break;
	}
}

void Button::start_selection()
{
	selected = true;
}

void Button::end_selection()
{
	selected = false;
}

void Button::start_highlight()
{
	highlighted = true;
}

void Button::end_highlight()
{
	highlighted = false;
}

#undef CLASSNAME
#define CLASSNAME Button

void Button::bind_methods()
{
	REG_SIGNAL("clicked");
}