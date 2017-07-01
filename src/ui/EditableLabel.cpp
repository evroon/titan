#include "EditableLabel.h"

#include "input/Input.h"
#include "input/Cursor.h"

EditableLabel::EditableLabel() : EditableLabel("")
{
	
}

EditableLabel::EditableLabel(const String &name)
{
	text = name;
	init();
}

EditableLabel::EditableLabel(const Variable & p_variable) : PropertyControl(p_variable)
{
	init();
}

void EditableLabel::init()
{
	start_margin = end_margin = 5;

	set_cursor_index(0);

	selection_color = DEFAULT_THEME->get_selection_color();

	ibeam_blinker = new UITimer(new UICallback(std::bind(&EditableLabel::switch_blink, this)), 500000);

	value_changed();
}

void EditableLabel::switch_blink()
{
	ibeam_blinks = !ibeam_blinks;
}

vec2 EditableLabel::get_required_size() const
{
	Font* f = CanvasData::get_singleton()->get_default_theme()->get_font();

	return vec2(f->get_width(text) + start_margin + end_margin, f->get_height());
}

void EditableLabel::handle_event(UIEvent *ui_event)
{
	Cursor cursor;
	int sel_pos;

	switch (ui_event->type)
	{
		case UIEvent::MOUSE_ENTER:

			cursor.set_cursor(Cursor::IBEAM);
			cursor.activate();
			break;

		case UIEvent::MOUSE_EXIT:

			cursor.set_default();
			cursor.activate();
			break;

		case UIEvent::MOUSE_PRESS:

			sel_pos = get_index(ui_event->pos.x);

			if (ui_event->press_type == Event::UP)
			{
				if (sel_pos == selection_first)
					selecting = false;

				if (!in_area(ui_event->pos))
				{
					cursor.set_default();
					cursor.activate(); 
				}
			}
			else
			{
				selection_first = sel_pos;
				selection_start = sel_pos;
				selection_end = sel_pos;

				update_selection_box();

				selecting = true;

				set_cursor_index(sel_pos);
			}

			break;

		case UIEvent::MOUSE_DOUBLE_CLICK:

			select_all();
			set_cursor_index(selection_end);
			break;

		case UIEvent::MOUSE_HOVER:

			if (MOUSE->is_pressed(Mouse::LEFT))
			{
				sel_pos = get_index(ui_event->pos.x);
				set_cursor_index(sel_pos);

				set_selection_end(sel_pos);
			}

			break;

		case UIEvent::FOCUS_START:

			INPUT->enable_text_input();
			break;

		case UIEvent::FOCUS_LOSE:

			INPUT->disable_text_input();
			break;

		case UIEvent::TEXT_INPUT:
			if (selecting)
				delete_selection();

			handle_input(ui_event->text[0]);
			break;

		case UIEvent::KEY_PRESS:

			if (ui_event->press_type == Event::DOWN)
			{
				switch (ui_event->key.key_type)
				{
					case Key::KEY_LEFT:
						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_left();
								set_selection_end(cursor_index);
							}
							else
							{
								selecting = true;
								set_selection_begin(cursor_index);
								move_cursor_left();
								set_selection_end(cursor_index);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_left();
						}
						break;

					case Key::KEY_RIGHT:
						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_right();
								set_selection_end(cursor_index);
							}
							else
							{
								selecting = true;
								set_selection_begin(cursor_index);
								move_cursor_right();
								set_selection_end(cursor_index);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_right();
						}
						break;

					case Key::KEY_UP:
						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							selecting = true;
							set_selection_begin(cursor_index);
							move_cursor_begin();
							set_selection_end(cursor_index);
						}
						else
						{
							stop_selecting();
							move_cursor_begin();
						}

						break;

					case Key::KEY_DOWN:
						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							selecting = true;
							set_selection_begin(cursor_index);
							move_cursor_end();
							set_selection_end(cursor_index);
						}
						else
						{
							stop_selecting();
							move_cursor_end();
						}
						break;

					case Key::KEY_A:
						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
						{
							selecting = true;
							set_selection_begin(0);
							move_cursor_end();
							set_selection_end(cursor_index);
						}
						break;

					case Key::KEY_X:

						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
							SDL_SetClipboardText(get_selected_text().c_str());

						delete_selection();

						break;

					case Key::KEY_C:
						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
							SDL_SetClipboardText(get_selected_text().c_str());
						break;

					case Key::KEY_V:

						stop_selecting();

						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
						{
							String inserted_text = SDL_GetClipboardText();
							insert_at_index(cursor_index, inserted_text);
							set_cursor_index(cursor_index + inserted_text.size());
						}
						break;

					case Key::KEY_BACKSPACE:

						if (selecting)
							delete_selection();
						else
						{
							remove_at_index(cursor_index - 1);
							move_cursor_left();
						}

						break;

					case Key::KEY_DELETE:

						if (cursor_is_at_end())
							return;

						if (selecting)
							delete_selection();
						else
							remove_at_index(cursor_index);
						set_cursor_index(cursor_index);

						break;

					case Key::KEY_RETURN:

						emit_signal("value_entered");

						break;
				}
			}
			break;
	}
}
 
void EditableLabel::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		if (get_focused() && selecting)
			draw_box(selection_area, selection_color);

		draw_text(text, vec2(area.get_left() + start_margin, area.pos.y));

		if (get_focused() && ibeam_blinks)
			draw_box(cursor_area, Color::Grey);

		if (text.size() == 0)
			draw_text(get_default_font(), empty_text, vec2(area.get_left() + start_margin, area.pos.y), TO_RGB(150));
		
		break;
	}
}

void EditableLabel::move_cursor_begin()
{
	set_cursor_index(0);
}

void EditableLabel::move_cursor_end()
{
	set_cursor_index(get_text().size());
}

void EditableLabel::move_cursor_left()
{
	set_cursor_index(cursor_index - 1);
}

void EditableLabel::move_cursor_right()
{
	set_cursor_index(cursor_index + 1);
}

void EditableLabel::set_cursor_index(int p_cursor_index)
{
	cursor_index = p_cursor_index;

	if (cursor_index < 0)
		cursor_index = 0;
	else if (cursor_index >(int)get_text().size())
		cursor_index = get_text().size();

	float position_x = get_position_x(cursor_index);

	cursor_area = rect2(
		position_x - 1,
		position_x + 1,
		area.get_top(),
		area.get_bottom());
}

void EditableLabel::insert_at_index(int index, char kar)
{
	if (index < 0 || index > get_text().size() + 1)
		return;

	set_text(get_text().insert(index, 1, kar));
}

void EditableLabel::insert_at_index(int index, const String &kar)
{
	if (index < 0 || index > get_text().size() + 1)
		return;

	set_text(get_text().insert(index, kar));
}

void EditableLabel::remove_at_index(int index)
{
	if (index < 0 || index > get_text().size() + 1)
		return;

	set_text(get_text().erase(index, 1));
}

bool EditableLabel::cursor_is_at_end()
{
	return cursor_index == get_text().size();
}

void EditableLabel::value_changed()
{
	Variant v = get_value();

	if (v.type == Variant::STRING)
		set_text(v);
}

String EditableLabel::get_selected_text()
{
	return get_text().substr(selection_start, selection_end - selection_start);
}

String EditableLabel::get_text() const
{
	return text;
}

void EditableLabel::set_text(const String &p_text)
{
	text = p_text;
}

inline String EditableLabel::get_empty_text() const
{
	return empty_text;
}

inline void EditableLabel::set_empty_text(const String & p_empty_text)
{
	empty_text = p_empty_text;
}

int EditableLabel::get_index(float x) const
{
	float offset = x - area.get_left() - start_margin;

	Font* f = CanvasData::get_singleton()->get_default_theme()->get_font();
	return f->get_index(text, offset);
}

float EditableLabel::get_position_x(int index) const
{
	float origin_x = area.get_left() + start_margin;
	Font* f = CanvasData::get_singleton()->get_default_theme()->get_font();
	return (origin_x + (float)f->get_width(text.substr(0, index)));
}
void EditableLabel::select_all()
{
	selecting = true;
	selection_first = selection_start = 0;
	selection_end = text.length();

	update_selection_box();
}
void EditableLabel::delete_selection()
{
	set_text(get_text().erase(selection_start, selection_end - selection_start));
	set_cursor_index(selection_start);
	stop_selecting();
}
void EditableLabel::set_selection_begin(int begin)
{
	selection_first = begin;
	update_selection_box();
}
void EditableLabel::set_selection_end(int end)
{
	if (selection_first > end)
	{
		selection_start = end;
		selection_end = selection_first;
	}
	else
	{
		selection_start = selection_first;
		selection_end = end;
	}

	update_selection_box();
}
void EditableLabel::stop_selecting()
{
	selecting = false;
	selection_start = selection_end = selection_first = 0;
}
void EditableLabel::update_selection_box()
{
	float position_1 = get_position_x(selection_start);
	float position_2 = get_position_x(selection_end);

	selection_area = rect2(
		position_1,
		position_2,
		get_area().get_top(),
		get_area().get_bottom()
	);
}

void EditableLabel::insert_at_selection(char c)
{
	insert_at_index(cursor_index, c);
	move_cursor_right();
}

void EditableLabel::handle_input(char c)
{
	insert_at_selection(c);
}

#undef CLASSNAME
#define CLASSNAME EditableLabel

void EditableLabel::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(String);

	REG_SIGNAL("value_entered");
}