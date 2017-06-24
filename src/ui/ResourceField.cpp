#include "ResourceField.h"

#include "TextField.h"
#include "TextButton.h"
#include "Dialog.h"


//=========================================================================
//ColorField
//=========================================================================

ColorField::ColorField()
{
	textbutton = new TextButton("...");
	add_child(textbutton);

	textbutton->set_margins(30, 4, 4, 4);
	textbutton->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

	textbutton->connect("clicked", this, "show_dialog");
}

ColorField::~ColorField()
{
}

ColorField::ColorField(Color& p_color) : ColorField()
{
	color = &p_color;
}

vec2 ColorField::get_required_size() const
{
	return vec2(100, get_default_font()->get_height());
}

void ColorField::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		rect2 color_area = area.crop(0, 40, 0, 0);

		draw_box(color_area, *color);

		break;
	}
}

void ColorField::show_dialog()
{
	color_dialog = new ColorPickDialog(*color);
	color_dialog->show();
	color_dialog->connect_signal("color_chosen", this, "color_chosen");

}

#undef CLASSNAME
#define CLASSNAME ColorField

void ColorField::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(Color);

	REG_METHOD(show_dialog);

	REG_SIGNAL("color_chosen");
}

//=========================================================================
//Texture2DField
//=========================================================================

Texture2DField::Texture2DField()
{
	textfield = new TextField;
	add_child(textfield);

	textfield->set_margins(4, 4, 40, 4);
	textfield->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

	textbutton = new TextButton("...");
	add_child(textbutton);

	textbutton->set_margins(30, 4, 4, 4);
	textbutton->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
}

Texture2DField::Texture2DField(Texture2D * p_texture) : Texture2DField()
{
	texture = p_texture;

	textfield->set_text(texture->get_file().get_name());
}

Texture2DField::~Texture2DField()
{
}

vec2 Texture2DField::get_required_size() const
{
	return vec2(100, get_default_font()->get_height());
}

#undef CLASSNAME
#define CLASSNAME Texture2DField

void Texture2DField::bind_methods()
{

}