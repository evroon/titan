#include "LabelButton.h"


LabelButton::LabelButton() : LabelButton("")
{
}

LabelButton::LabelButton(const String &text) : Button(), label(text)
{
	
}

vec2 LabelButton::get_required_size() const
{
	return label.get_required_size() + vec2(5);
}

void LabelButton::draw()
{
	if (selected || highlighted)
		box.draw();

	label.draw();
}

void LabelButton::start_selection()
{
	Button::start_selection();
	box.set_color(Color::FromRGB(vec3i(20)));
}

void LabelButton::end_selection()
{
	Button::end_selection();
	box.set_color(Color::FromRGB(vec3i(90)));
}

void LabelButton::start_highlight()
{
	Button::start_highlight();
	box.set_color(Color::FromRGB(vec3i(90)));
}

void LabelButton::end_highlight()
{
	Button::end_highlight();
	box.set_color(Color::FromRGB(vec3i(50)));
}

#undef CLASSNAME
#define CLASSNAME LabelButton

void LabelButton::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(String);

	REG_CSTR_OVRLD_1(String);
}