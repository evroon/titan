#include "ComboBox.h"

#include "graphics/View.h"

ComboBox::ComboBox()
{
	menu = new ContextMenu;
}

ComboBox::~ComboBox()
{

}

vec2 ComboBox::get_required_size() const
{
	return vec2(200, 20);
}

void ComboBox::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		rect2 work_area = area;
		work_area.size.y = get_default_font()->get_height() / 2.0f + 8;

		draw_frame(work_area, TO_RGB(80));

		float offset = work_area.get_left() + 4;
		if (texture)
		{
			//draw_texture(texture, )
			//offset += texture.get_size().x;
		}
		
		if (text.size() > 0)
		{
			draw_text(text, vec2(offset, work_area.pos.y));
		}

		break;
	}
}

void ComboBox::add_item(const String& p_text)
{
	menu->add_item(p_text, Connection());
	update();
}

void ComboBox::set_text(const String & p_text)
{
	text = p_text;
	update();
}

void ComboBox::selected()
{
	int index = menu->get_selected();
	text = menu->get_text(index);
	emit_signal("selected", text);
	update();
}

void ComboBox::clear()
{
	menu->clear();
	update();
}

void ComboBox::handle_event(UIEvent* p_ui_event)
{
	switch (p_ui_event->type)
	{
	case UIEvent::FOCUS_START:
		ACTIVE_CANVAS->set_context_menu(menu, area.get_bottom_left());
		menu->connect("selected", this, "selected");
		break;

	case UIEvent::FOCUS_LOSE:
		ACTIVE_CANVAS->remove_context_menu();
	}
}

#undef CLASSNAME
#define CLASSNAME ComboBox

void ComboBox::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(add_item);
	REG_METHOD(selected);
	REG_SIGNAL("selected");
}