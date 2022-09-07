#include "framedbutton.h"



FramedButton::FramedButton()
{
	set_color(Color::FromRGB(vec3i(90)));
}


FramedButton::~FramedButton()
{

}

void FramedButton::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		draw_frame(area, color);

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		update();
		break;
	}
}

void FramedButton::set_color(const Color& p_color)
{
	color = p_color;
}

void FramedButton::start_highlight()
{
	Button::start_highlight();
	set_color(Color::FromRGB(vec3i(120)));
}

void FramedButton::start_selection()
{
	Button::start_selection();
	set_color(Color::FromRGB(vec3i(50, 120, 170)));
}

void FramedButton::end_highlight()
{
	Button::end_highlight();
	set_color(Color::FromRGB(vec3i(90)));
}

void FramedButton::end_selection()
{
	Button::end_selection();
	set_color(Color::FromRGB(vec3i(120)));
}