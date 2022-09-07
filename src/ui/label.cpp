#include "label.h"

#include "canvas.h"
#include "core/tmessage.h"

Label::Label() : Label("Label")
{

}

Label::Label(const String& t) : Label(t, CanvasData::get_singleton()->get_default_theme()->get_font())
{
}

Label::Label(const String &t, Font *f)
{
	set_font(f);
	text = t;
}

void Label::handle_event(UIEvent *ui_event)
{

}

void Label::set_font(Font * p_font)
{
	font = p_font;
}

Font* Label::get_font() const
{
	return font;
}
void Label::set_color(const Color & p_color)
{
	color = p_color;
}
Color Label::get_color() const
{
	return color;
}
int Label::get_index(float x) const
{
	float offset = x - area.get_left();

	return font->get_index(text, offset);
}
float Label::get_position_x(int index) const
{
	float origin_x = area.get_left();
	return (origin_x + (float)get_font()->get_width(text.substr(0, index)));
}

void Label::set_centering_type(const Image::CenteringType &centering_type)
{
	//image.centertype = centering_type;
}

String Label::get_text() const
{
	return text;
}

vec2 Label::get_required_size() const
{
	Font* f = CanvasData::get_singleton()->get_default_theme()->get_font();

	return vec2(f->get_width(text), f->get_height());
}

bool Label::is_in_text(const vec2 &pos)
{
	//return image.in_render_area(pos);

	return true;
}

void Label::set_text(const String &p_text)
{
	text = p_text;

	if (text == "")
		text = " ";

	update();
}

void Label::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:



		break;

	case NOTIFICATION_DRAW:

		draw_text(font, text, vec2(area.get_left(), area.pos.y), color);

		break;
	}
}

#undef CLASSNAME
#define CLASSNAME Label

void Label::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(String);
	REG_CSTR(2);

	REG_PROPERTY(text);
}