#include "ListView.h"

#include "Canvas.h"
#include "graphics/Renderer.h"

#include "Slider.h"

#include "input/Keyboard.h"


ListElement::ListElement(const String &p_text)
{
	text = p_text;
	icon = NULL;
}

ListElement::~ListElement()
{

}

void ListElement::bind_textbox(ListView *p_textbox)
{
	listview = p_textbox;
}

void ListElement::set_index(int p_index)
{
	index = p_index;
}

int ListElement::get_index()
{
	return index;
}

rect2 ListElement::get_area() const
{
	return area;
}

void ListElement::set_area(const rect2 & p_area)
{
	area = p_area;
}

bool ListElement::is_in_area(const vec2 & p_pos)
{
	return visible && area.is_in_box(p_pos);
}

String ListElement::get_text() const
{
	return text;
}

void ListElement::set_text(const String & p_text)
{
	text = p_text;
}

Texture2D * ListElement::get_icon() const
{
	return icon;
}

void ListElement::set_icon(Texture2D * p_icon)
{
	icon = p_icon;
}

void ListElement::set_visible(bool p_visible)
{
	visible = p_visible;
}

bool ListElement::get_visible() const
{
	return visible;
}

float ListElement::get_height() const
{
	return CanvasData::get_singleton()->get_default_theme()->get_font()->get_height();
}

int ListElement::get_index(float x) const
{
	float offset = x - area.get_left();

	return listview->get_font()->get_index(text, offset);
}
float ListElement::get_position_x(int index) const
{
	float origin_x = area.get_left();
	return (origin_x + (float)listview->get_font()->get_width(text.substr(0, index)));
}


//=========================================================================
//ListView
//=========================================================================

ListView::ListView()
{
	left_margin = 4;
	right_margin = 4;
	top_margin = 3;

	internal_left_margin = 4;

	background_color = TO_RGB(50);
	selection_color = TO_RGB(170);
	item_numbers_color = TO_RGB(200);
	multi_select_color = DEFAULT_THEME->get_selection_color();
	multi_select_color.a = 0.7f;

	font = CanvasData::get_singleton()->get_default_theme()->get_font();

	extra_space = 100;

	slider = NULL;

	selected = -1;
	highlighted = -1;
	selection_begin = -1;
	selection_end = -1;
}

ListView::~ListView()
{

}

vec2 ListView::get_required_size() const
{
	return vec2(100);
}

void ListView::handle_event(UIEvent *ui_event)
{
	switch (ui_event->type)
	{
	case UIEvent::MOUSE_ENTER:

		break;

	case UIEvent::MOUSE_EXIT:

		highlighted = -1;

		break;

	case UIEvent::FOCUS_START:

		break;

	case UIEvent::FOCUS_LOSE:

		break;

	case UIEvent::MOUSE_SCROLL:

		if (slider)
		{
			if (ui_event->scroll_type == Event::SCROLL_DOWN)
				slider->step_down();
			else
				slider->step_up();
		}

		break;

	case UIEvent::MOUSE_HOVER:

		highlighted = get_position(ui_event->pos);

		break;

	case UIEvent::MOUSE_PRESS:

		if (ui_event->press_type == UIEvent::DOWN)
		{
			int index = get_position(ui_event->pos);
			
			if (index == -1)
				select_none();
			else
			{
				if (KEYBOARD->is_button_pressed(Key::KEY_LSHIFT))
				{
					if (selection_first == -1)
						selection_first = selected;
					
					if (selection_first == -1)
						selection_first = 0;
					
					selecting = true;
					set_selection(selection_first, index);
				}
				else
					select_none();

				set_selected(index);
			}		
		}

		break;

	case UIEvent::MOUSE_DOUBLE_CLICK:

		emit_signal("chosen", selected);

		break;

	case UIEvent::KEY_PRESS:

		if (ui_event->press_type == Event::DOWN)
		{
			switch (ui_event->key.key_type)
			{
			case Key::KEY_HOME:
				if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
				{
					selecting = true;
					set_selection_begin(selected);
					move_selected_begin();
					set_selection_end(selected);
				}
				else
				{
					stop_selecting();
					move_selected_begin();
				}

				break;

			case Key::KEY_END:
				if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
				{
					selecting = true;
					set_selection_begin(selected);
					move_selected_end();
					set_selection_end(selected);
				}
				else
				{
					stop_selecting();
					move_selected_end();
				}
				break;

			case Key::KEY_UP:
				if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
				{
					if (selected == 0)
						return;

					if (!selecting)
						set_selection_begin(selected);

					move_selected_up();
					set_selection_end(selected);
					selecting = true;
				}
				else
				{
					stop_selecting();
					move_selected_up();
				}
				break;

			case Key::KEY_DOWN:

				if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
				{
					if (selected == items.size() - 1)
						return;

					if (!selecting)
						set_selection_begin(selected);

					move_selected_down();
					set_selection_end(selected);
					selecting = true;
				}
				else
				{
					stop_selecting();
					move_selected_down();
				}

				break;

			case Key::KEY_RETURN:
				
				emit_signal("chosen", selected);

				break;

			case Key::KEY_DELETE:

				//move_selected_right();
				//remove_at_index(caret_pos);
				//move_selected_left();

				break;

			case Key::KEY_A:

				if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
				{
					select_all();
				}

				break;
			}
		}

		break;

	case UIEvent::TEXT_INPUT:

		//insert_at_selection(ui_event->text[0]);

		break;
	}
}

void ListView::position_items()
{
	float offset_x = area.get_left() + left_margin;
	float offset_y = area.get_top() - top_margin + scroll_offset;

	for (int c = 0; c < items.size(); c++)
	{
		ListElement& item = items[c];

		if (!item.get_visible())
			continue;

		rect2 a = rect2(offset_x, area.get_right() - right_margin, offset_y, offset_y - item.get_height());
		item.set_area(a);

		offset_y -= items[c].get_height();
	}
	check_slider_necessity();
}

void ListView::check_slider_necessity()
{
	if (items.size() > 0 && items[items.size() - 1].get_area().get_bottom() < area.get_bottom())
	{
		extra_space = area.get_bottom() - items[items.size() - 1].get_area().get_bottom();
		add_slider();
	}
	else
		remove_slider();

}

void ListView::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		break;

	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		position_items();

		break;

	case NOTIFICATION_DRAW:

		draw_frame(area, TO_RGB(30));


		RENDERER->use_scissor(area);
		
		if (selecting && selection_begin < items.size() && selection_end < items.size() && selection_begin >= 0 && selection_end >= 0)
		{
			float top = get_item(selection_begin)->get_area().get_top();
			float bottom = get_item(selection_end)->get_area().get_bottom();
			float left = get_item(selection_begin)->get_area().get_left();
			float right = get_item(selection_begin)->get_area().get_right();

			draw_box(rect2(left, right, top, bottom), multi_select_color);
		}

		if (highlighted != -1)
			draw_box(get_item(highlighted)->get_area(), DEFAULT_THEME->get_highlight_color());

		if (selected != -1)
			draw_box(get_item(selected)->get_area(), DEFAULT_THEME->get_selection_color());

		for (int c = 0; c < items.size(); c++)
		{
			if (items[c].get_area().get_bottom() > area.get_top())
				continue;

			if (item_numbers_enabled)
				draw_text(font, c, vec2(area.get_left() + 4, items[c].get_area().pos.y), item_numbers_color);

			if (items[c].get_icon())
			{
				vec2 size = items[c].get_icon()->get_size() / 2.0f;
				vec2 pos = vec2(items[c].get_area().get_left() + internal_left_margin + size.x, items[c].get_area().pos.y);

				draw_texture(items[c].get_icon(), rect2(pos, size), Color::White);
				draw_text(items[c].get_text(), vec2(pos.x + size.x + 4, pos.y));
			}
			else
				draw_text(items[c].get_text(), vec2(items[c].get_area().get_left() + internal_left_margin, items[c].get_area().pos.y));
		}

		if (items.size() == 0)
		{
			float width = get_font()->get_width("No content to display");

			draw_text("No content to display", area.pos - vec2(width / 2.0f, 0));
		}

		RENDERER->stop_scissor();

		if (get_focused())
			draw_frame(DEFAULT_THEME->get_highlight(), area, Color::Green);

		break;
	}
}

void ListView::push_back_item(ListElement& item)
{
	int index = items.size();

	item.bind_textbox(this);
	item.set_index(index);

	items.push_back(item);

	position_items();
}

void ListView::clear()
{
	items.clear();

	selected = -1;
	highlighted = -1;

	remove_slider();
}

int ListView::get_position(const vec2 &p_pos)
{
	if (!area.is_in_box(p_pos))
		return -1;
	
	for (int c = 0; c < items.size(); c++)
	{
		if (items[c].is_in_area(p_pos))
			return c;
	}
	return -1;
}

ListElement* ListView::get_item(int row)
{
	if (row < 0)
		row = 0;
	else if (row > int(items.size()) - 1)
		row = items.size() - 1;

	return &items.get(row);
}

int ListView::get_item_size(int row)
{
	ListElement* l = get_item(row);

	if (l)
		return get_item(row)->get_text().size();

	return -1;
}

void ListView::set_item_numbers_enabled(bool p_value)
{
	item_numbers_enabled = p_value;

	if (item_numbers_enabled)
		left_margin = 15;
	else
		left_margin = 5;

	position_items();

}

void ListView::add_slider()
{
	if (slider)
		return;

	slider = new Slider;
	add_child(slider);

	slider->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	slider->set_margins(20.0f, 5.0f, 5.0f, 5.0f);

	Connection c;
	c.register_native_method(this, "slider_value_changed");
	slider->connect("value_changed", c);

	right_margin = 24.0f;

	position_items();
}

void ListView::remove_slider()
{
	if (!slider)
		return;

	remove_child(slider);
	delete slider;
	slider = NULL;

	right_margin = 4.0f;
	scroll_offset = 0.0f;

	position_items();
}

void ListView::set_selected(int p_row)
{
	if (selected == p_row)
		return;

	selecting = true;

	selected = p_row;
	emit_signal("selected", selected);

	make_visible(p_row);
}

int ListView::get_selected() const
{
	return selected;
}

void ListView::move_selected_begin()
{
	set_selected(0);
}

void ListView::move_selected_end()
{
	set_selected(items.size() - 1);
}

void ListView::move_selected_up()
{
	if (selected == 0)
		return;

	set_selected(selected - 1);
}

void ListView::move_selected_down()
{
	if (selected == items.size() - 1)
		return;

	set_selected(selected + 1);
}

void ListView::set_selection(int p_selected)
{
	selecting = true;
	selected = p_selected;

	selection_begin = 0;
	selection_end = 0;
}

void ListView::set_selection(int p_begin, int p_end)
{
	if (p_begin == p_end)
	{
		set_selection(p_begin);
		return;
	}

	if (p_begin > p_end)
	{
		int temp = p_begin;
		p_begin = p_end;
		p_end = temp;
	}

	selection_begin = p_begin;
	selection_end = p_end;
	selecting = true;

	emit_signal("selected", selected);
}

void ListView::delete_selection()
{
	if (selected != -1)
		remove_item(selected);
	else
	{
		for (int c = selection_begin; c <= selection_end; c++)
			remove_item(c);
	}
}

void ListView::set_selection_begin(int p_begin)
{
	selection_first = p_begin;
	update();
}

void ListView::set_selection_end(int p_end)
{
	if (selection_first > p_end)
	{
		selection_begin = p_end;
		selection_end = selection_first;
	}
	else
	{
		selection_begin = selection_first;
		selection_end = p_end;
	}

	update();
}

void ListView::stop_selecting()
{
	if (!selecting)
		return;

	selecting = false;
	selection_begin = selection_end = selection_first = 0;

	emit_signal("selected", selected);
}

void ListView::select_all()
{
	if (selecting && selection_begin == 0 && selection_end == items.size() - 1)
		return;

	set_selection(0, items.size() - 1);

	emit_signal("selected", selected);
}

void ListView::select_none()
{
	if (!selecting)
		return;

	selecting = false;
	selection_begin = selection_end = selection_first = -1;
	selected = -1;

	emit_signal("selected", selected);
}

bool ListView::multiple_selection() const
{
	return selecting && selection_begin != 0 && selection_begin != selection_end;
}

void ListView::make_visible(int p_index)
{
	rect2 item_area = get_item(p_index)->get_area();

	float delta = 0.0f;

	if (item_area.get_bottom() < area.get_bottom())
		delta = item_area.get_bottom() - area.get_bottom();

	if (item_area.get_top() > area.get_top())
		delta = item_area.get_top() - area.get_top();

	if (slider)
		slider->set_slider_pos(slider->get_value() - delta / extra_space);
}

void ListView::search(const String& p_src)
{
	for (int c = 0; c < items.size(); c++)
	{
		ListElement& item = items[c];

		item.set_visible(item.get_text().starts_with(p_src));
	}
}

void ListView::stop_search()
{
	for (int c = 0; c < items.size(); c++)
		items[c].set_visible(true);
}

void ListView::slider_value_changed()
{
	scroll_offset = slider->get_value() * extra_space;

	position_items();
	update();
}

void ListView::remove_item(int p_index)
{
	items.clear(p_index);

	if (p_index == selected)
		selected = -1;

	if (p_index == highlighted)
		highlighted = -1;

	position_items();
}

void ListView::push_back_item(const String & p_text, Texture2D* p_icon)
{
	int index = items.size();

	ListElement item(p_text);

	if (p_icon)
		item.set_icon(p_icon);

	item.bind_textbox(this);
	item.set_index(index);

	items.push_back(item);

	position_items();
}

void ListView::insert_item_at_index(int index, ListElement& item)
{
	item.bind_textbox(this);
	item.set_index(index);

	if (index >= items.size())
	{
		push_back_item(item);
	}
	else
	{
		ListElement& old = items[index];

		items.replace(index, item);

		items.push_back(old);
	}
}

#undef CLASSNAME
#define CLASSNAME ListView

void ListView::set_font(Font* p_font)
{
	font = p_font;
}

Font* ListView::get_font() const
{
	return font;
}

void ListView::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(slider_value_changed);
	REG_SIGNAL("selected");
	REG_SIGNAL("chosen");
}