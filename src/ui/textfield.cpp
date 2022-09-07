#include "textfield.h"

#include "canvas.h"

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

vec2 TextField::get_required_size() const
{
	return EditableLabel::get_required_size() + vec2(0, 8);
}

void TextField::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:
	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		update_value();

		break;

	case NOTIFICATION_DRAW:

		draw_frame(area, Color::DarkGrey);

		EditableLabel::notification(NOTIFICATION_DRAW);

		if (get_focused())
			draw_frame(CanvasData::get_singleton()->get_default_theme()->get_highlight(), area, Color::Green);

		break;
	}
}

void TextField::value_changed()
{
	Variant v = get_value();

	if (v.type == Variant::FLOAT)
		set_text(v.ToString());
}

void TextField::handle_input(char c)
{
	insert_at_selection(c);
}

void TextField::set_text(const String& p_text)
{
	text = p_text;
	value_set(p_text);

	emit_signal("value_changed", p_text);
}

#undef CLASSNAME
#define CLASSNAME TextField

void TextField::bind_methods()
{

}