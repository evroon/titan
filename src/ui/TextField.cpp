#include "TextField.h"

#include "Canvas.h"

TextField::TextField() : TextField("")
{

}

TextField::TextField(const String &name) : EditableLabel(name)
{

}

TextField::TextField(const Variable & p_variable) : EditableLabel(p_variable)
{
}

TextField::~TextField()
{
}

void TextField::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		draw_frame(area, Color::DarkGrey);

		EditableLabel::notification(NOTIFICATION_DRAW);

		if (get_focused())
			draw_frame(CanvasData::get_singleton()->get_default_theme()->get_highlight(), area, Color::Green);

		break;
	}
}

void TextField::handle_input(char c)
{
	insert_at_selection(c);
}

void TextField::set_text(const String & p_text)
{
	text = p_text;
}

#undef CLASSNAME
#define CLASSNAME TextField

void TextField::bind_methods()
{

}