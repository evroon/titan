#include "TextBox.h"

#include "Label.h"
#include "input/Input.h"
#include "input/Cursor.h"
#include "graphics/Renderer.h"
#include "Graphics/View.h"
#include "ContextMenu.h"

//=========================================================================
//TextLine
//=========================================================================

TextLine::TextLine(const String &p_text)
{
	text = p_text;
	styles.push_back({ Color::White, 0 });
}

TextLine::~TextLine()
{

}

void TextLine::bind_textbox(TextBox *p_textbox)
{
	textbox = p_textbox;
}

void TextLine::set_index(int p_index)
{
	index = p_index;
}

int TextLine::get_index()
{
	return index;
}

rect2 TextLine::get_area() const
{
	return area;
}

void TextLine::set_area(const rect2 & p_area)
{
	area = p_area;
}

bool TextLine::is_in_text(const vec2 & p_pos)
{
	return area.is_in_box(p_pos);
}

String TextLine::get_text() const
{
	return text;
}

void TextLine::set_text(const String & p_text)
{
	text = p_text;
}

float TextLine::get_height() const
{
	return CanvasData::get_singleton()->get_default_theme()->get_font()->get_height();
}

int TextLine::get_index(float x) const
{
	float offset = x - area.get_left();

	return textbox->get_font()->get_index(text, offset);
}
float TextLine::get_position_x(int index) const
{
	float origin_x = area.get_left();
	return (origin_x + (float)textbox->get_font()->get_width(text.substr(0, index)));
}


//=========================================================================
//TextBox
//=========================================================================

TextBox::TextBox() : TextBox("")
{
	
}

TextBox::TextBox(const String & p_source)
{
	left_margin = 5;
	top_margin = 3;
	right_margin = 3;

	background_color = TO_RGB(40);
	cursor_color = TO_RGB(170);
	line_numbers_color = TO_RGB(200);
	selection_color = DEFAULT_THEME->get_selection_color();
	selection_color.a = 0.8f;

	ibeam_blinker = new UITimer(new UICallback(std::bind(&TextBox::switch_blink, this)), 500000);
	ibeam_blinker->set_enabled(false);

	font = CanvasData::get_singleton()->get_default_theme()->get_font();

	slider = NULL;

	menu = new ContextMenu;

	menu->add_item("Copy");
	menu->add_item("Cut");
	menu->add_item("Paste");
	menu->add_seperator();
	menu->add_item("Je moeder");

	menu->connect("selected", this, "contextmenu_selected");

	set_text(p_source);

	give_color_to_word("Reading", Color::Red);
	give_color_to_word("Output", Color::Blue);
}


TextBox::~TextBox()
{
}

void TextBox::switch_blink()
{
	ibeam_blinks = !ibeam_blinks;
}

vec2 TextBox::get_required_size() const
{
	return vec2(100);
}

void TextBox::handle_event(UIEvent *ui_event)
{
	Cursor cursor;
	TextPosition sel_pos;

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

		case UIEvent::FOCUS_START:

			INPUT->enable_text_input();
			ibeam_blinker->set_enabled(true);
			ibeam_blinks = true;

			break;

		case UIEvent::FOCUS_LOSE:

			INPUT->disable_text_input();
			ibeam_blinker->set_enabled(false);
			ibeam_blinks = false;

			break;

		case UIEvent::MOUSE_SCROLL:

			if (slider)
			{
				slider->handle_event(ui_event);
			}

			update_caret();
			update();

			break;

		case UIEvent::MOUSE_PRESS:

			if (ui_event->button_type == Mouse::RIGHT)
			{
				if (ui_event->press_type == UIEvent::DOWN)
				{
					ACTIVE_CANVAS->set_context_menu(menu, ui_event->pos);
				}

				set_caret_pos(get_position(ui_event->pos));
				break;
			}

			sel_pos = get_position(ui_event->pos);

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
				selection_begin = sel_pos;
				selection_end = sel_pos;

				selecting = true;

				set_caret_pos(sel_pos);
			}

			break;

		case UIEvent::MOUSE_HOVER:

			if (MOUSE->is_pressed(Mouse::LEFT))
			{
				sel_pos = get_position(ui_event->pos);
				
				set_caret_pos(sel_pos);

				set_selection_end(sel_pos);
			}

			break;

		case UIEvent::KEY_PRESS:

			if (ui_event->press_type == Event::DOWN)
			{
				switch (ui_event->key.key_type)
				{
					case Key::KEY_HOME:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_begin();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_begin();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_begin();
						}

						break;

					case Key::KEY_END:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_end();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_end();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_end();
						}

						break;

					case Key::KEY_UP:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_up();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_up();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_up();
						}

						break;

					case Key::KEY_DOWN:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_down();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_down();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							stop_selecting();
							move_cursor_down();
						}

						break;

					case Key::KEY_RIGHT:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_right();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_right();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							sel_pos = selection_end;
							stop_selecting();
							set_caret_pos(sel_pos);
							move_cursor_right();
						}
						break;

					case Key::KEY_LEFT:

						if (ui_event->mod.type & ModKey::KEY_MOD_SHIFT)
						{
							if (selecting)
							{
								move_cursor_left();
								set_selection_end(caret_pos);
							}
							else
							{
								selecting = true;
								set_selection_begin(caret_pos);
								move_cursor_left();
								set_selection_end(caret_pos);
							}
						}
						else
						{
							sel_pos = selection_begin;
							stop_selecting();
							set_caret_pos(sel_pos);
							move_cursor_left();
						}

						break;

					case Key::KEY_X:

						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
							cut_text();

						break;

					case Key::KEY_C:

						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
							copy_text();

						break;

					case Key::KEY_V:

						if (ui_event->mod.type & ModKey::KEY_MOD_CTRL)
							paste_text();

						break;

					case Key::KEY_RETURN:
						
						insert_in_line(caret_pos);
						
						break;

					case Key::KEY_BACKSPACE:

						if (selecting)
							delete_selection();
						else
						{
							remove_at_index(TextPosition(caret_pos.row, caret_pos.column));
							move_cursor_left();
						}

						break;

					case Key::KEY_DELETE:
						
						if (selecting)
							delete_selection();
						else
						{
							move_cursor_right();
							remove_at_index(caret_pos);
							move_cursor_left();
						}

						break;
				}
			}

			break;

		case UIEvent::TEXT_INPUT:

			insert_at_selection(ui_event->text[0]);

			break;
	}
}

void TextBox::position_lines()
{
	float offset_x = area.get_left() + left_margin;
	float offset_y = area.get_top() - top_margin + scroll_offset;

	for (int c = 0; c < lines.size(); c++)
	{
		TextLine& line = lines[c];

		rect2 a = rect2(offset_x, area.get_right() - right_margin, offset_y, offset_y - line.get_height());
		line.set_area(a);

		offset_y -= lines[c].get_height();
	}
}

void TextBox::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		set_caret_pos(TextPosition(0, 0));

		break;

	case NOTIFICATION_RESIZED:

		position_lines();
		check_slider_necessity();

		update_caret();

		if (slider)
			slider->flag_size_changed();

		break;


	case NOTIFICATION_DRAW:

		draw_frame(area, background_color);

		if (get_focused())
			draw_frame(DEFAULT_THEME->get_highlight(), area, Color::Green);

		RENDERER->use_scissor(area);
		
		//draw selection
		if (selecting && !(selection_begin ==  selection_end))
		{
			for (int c = selection_begin.row; c <= selection_end.row; c++)
			{
				TextLine* l = get_line(c);
				rect2 area = l->get_area();

				float left;
				float right;
				float top = area.get_top();
				float bottom = area.get_bottom();

				if (c == selection_begin.row)
					left = l->get_position_x(selection_begin.column);
				else
					left = area.get_left();

				if (c == selection_end.row)
					right = l->get_position_x(selection_end.column);
				else
					right = l->get_position_x(l->get_text().size());

				rect2 box = rect2(left, right, top, bottom);

				draw_box(box, selection_color);
			}
		}

		//draw text
		for (int c = 0; c < lines.size(); c++)
		{
			if (lines[c].get_area().get_bottom() > area.get_top())
				continue;

			if (lines[c].get_area().get_top() < area.get_bottom())
				break;

			if (line_numbers_enabled)
				draw_text(font, c, vec2(area.get_left() + 4, lines[c].get_area().pos.y), line_numbers_color);

			float offset = lines[c].get_area().get_left();

			for (int s = 0; s < lines[c].styles.size(); s++)
			{
				const TextStyle& style = lines[c].styles[s];

				int end = lines[c].get_text().size();
				if (s < lines[c].styles.size() - 1)
					end = lines[c].styles[s + 1].start;

				String text = lines[c].get_text().substr(style.start, end - style.start);

				draw_text(font, text, vec2(offset, lines[c].get_area().pos.y), style.color);
				offset += font->get_width(text);
			}
		}
		if (line_numbers_enabled)
			draw_line(area.get_upper_left() + vec2(left_margin - 3, 0), area.get_bottom_left() + vec2(left_margin - 3, 0), TO_RGB(200));
		
		if (ibeam_blinks)
			draw_box(cursor, Color::White);

		if (slider)
			slider->draw();

		RENDERER->stop_scissor();

		break;
	}
}

void TextBox::push_back_line(const String& p_source)
{
	push_back_line(TextLine(p_source));
}

void TextBox::push_back_line(TextLine& line)
{
	int index = lines.size();

	line.bind_textbox(this);
	line.set_index(index);

	lines.push_back(line);

	position_lines();
	check_slider_necessity();

	make_visible(index);
}

void TextBox::set_font(Font* p_font)
{
	font = p_font;
}

Font* TextBox::get_font() const
{
	return font;
}

void TextBox::clear_text()
{
	lines.clear();
}

String TextBox::get_text() const
{
	String result;

	for (int c = 0; c < lines.size(); c++)
		result += lines[c].get_text() + (c == lines.size() - 1 ? "" : "\n");

	return result;
}

void TextBox::set_text(const String &source)
{
	clear_text();

	String buffer = "";

	for (int i  = 0; i < source.size(); i++)
	{
		char c = source[i];

		if (c == '\n')
		{
			if (buffer != "")
				push_back_line(TextLine(buffer));

			buffer = "";
		}
		else
			buffer += c;
	}
	push_back_line(TextLine(buffer));

	set_caret_pos(TextPosition());
}

void TextBox::contextmenu_selected(int p_index)
{	
	switch (p_index)
	{
	case 0:

		copy_text();
		break;

	case 1:

		cut_text();
		break;

	case 2:

		paste_text();
		break;
	}
}

void TextBox::cut_text()
{
	SDL_SetClipboardText(get_selected_text().c_str());

	TextPosition begin = selection_begin;
	delete_selection();
	set_caret_pos(begin);
}

void TextBox::copy_text()
{
	SDL_SetClipboardText(get_selected_text().c_str());
}

void TextBox::paste_text()
{
	String inserted_text = SDL_GetClipboardText();
	insert_at_index(caret_pos, inserted_text);
	set_caret_pos(TextPosition(caret_pos.row, caret_pos.column + inserted_text.size()));
}

String TextBox::get_selected_text()
{
	String result;

	for (int c = selection_begin.row; c <= selection_end.row; c++)
	{
		String text = get_line(c)->get_text();

		int start = 0;
		int end = text.size();

		if (c == selection_begin.row)
			start = selection_begin.column;

		if (c == selection_end.row)
			end = selection_end.column;


		result += text.substr(start, end - start) + (c == lines.size() - 1 ? "" : "\n");
	}

	return result;
}

TextPosition TextBox::get_position(const vec2 &pos)
{
	TextPosition textpos;
	
	TextLine* line = NULL;
	int c = 0;

	for (; c < lines.size(); c++)
	{
		if (lines[c].get_area().is_in_box(pos))
		{
			line = &lines[c];
			break;
		}
	}

	if (!line)
	{
		if (pos.y >= lines[0].get_area().get_bottom())
			textpos.row = 0;
		else if (pos.y <= area.get_bottom())
			textpos.row = lines.size() - 1;
		else
		{
			if (pos.y <= lines[0].get_area().get_top() && pos.y >= lines[lines.size() - 1].get_area().get_bottom())
				textpos.row = get_position(vec2(area.pos.x, pos.y)).row;
			else
				textpos.row = lines.size() - 1;
		}

		line = get_line(textpos.row);

		if (pos.x >= area.get_right())
			textpos.column = line->get_text().size();

		else if (pos.x <= area.get_left())
			textpos.column = 0;
		else
			textpos.column = line->get_index(pos.x);

		return textpos;
	}

	textpos.row = c;
	textpos.column = line->get_index(pos.x);

	return textpos;
}

TextLine* TextBox::get_line(int row)
{
	if (row < 0)
		row = 0;
	else if (row > int(lines.size()) - 1)
		row = lines.size() - 1;

	return &lines[row];
}

int TextBox::get_line_size(int row)
{
	TextLine* l = get_line(row);

	if (l)
		return get_line(row)->get_text().size();

	return -1;
}

void TextBox::set_caret_pos(const TextPosition &p_pos, bool update_preferred)
{
	if (p_pos.row == -1 || p_pos.column == -1)
		return;

	caret_pos = p_pos;

	int row_count = lines.size();

	if (caret_pos.row < 0)
		caret_pos.row = 0;

	if (caret_pos.row > row_count - 1)
		caret_pos.row = row_count - 1;

	TextLine* line = get_line(caret_pos.row);
	int column_count = line->get_text().size() + 1;

	if (caret_pos.column < 0)
		caret_pos.column = 0;
	
	if (caret_pos.column > column_count)
		caret_pos.column = column_count;

	update_caret();

	if (update_preferred)
		preferred_offset_x = cursor.pos.x;

	make_visible(caret_pos.row);
}

void TextBox::update_caret()
{
	TextLine* line = get_line(caret_pos.row);
	float position_x = line->get_position_x(caret_pos.column);

	cursor = rect2(
		position_x - 1,
		position_x + 1,
		line->get_area().get_top(),
		line->get_area().get_bottom());
}

void TextBox::set_line_numbers_enabled(bool p_value)
{
	line_numbers_enabled = p_value;

	if (line_numbers_enabled)
		left_margin = 30;
	else
		left_margin = 5;

	position_lines();
	update_caret();
}

void TextBox::check_slider_necessity()
{
	extra_space = area.get_bottom() - lines[lines.size() - 1].get_area().get_bottom();

	if (lines.size() > 0 && extra_space > 0)
		add_slider();
	else
		remove_slider();

}

void TextBox::add_slider()
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
}

void TextBox::remove_slider()
{
	if (!slider)
		return;

	remove_child(slider);
	slider = NULL;

	extra_space = 0.0f;
	scroll_offset = 0.0f;

	position_lines();
	update_caret();
}

void TextBox::move_cursor_begin()
{
	set_caret_pos({ caret_pos.row, 0 });
}

void TextBox::move_cursor_end()
{
	set_caret_pos({ caret_pos.row, get_line_size(caret_pos.row) });
}

void TextBox::move_cursor_left()
{
	if (caret_pos.column <= 0)
		set_caret_pos({ caret_pos.row - 1, get_line(caret_pos.row - 1)->get_text().size() });
	else
		set_caret_pos({ caret_pos.row, caret_pos.column - 1});
}

void TextBox::move_cursor_right()
{
	if (caret_pos.column >= get_line_size(caret_pos.row))
	{
		if (caret_pos.row == lines.size() - 1)
			return;

		set_caret_pos({ caret_pos.row + 1, 0 });
	}
	else
		set_caret_pos({ caret_pos.row, caret_pos.column + 1 });
}

void TextBox::move_cursor_up()
{
	if (caret_pos.row == 0)
		return;

	float current_offset = cursor.pos.x;

	TextLine *line_below = get_line(caret_pos.row - 1);

	vec2 newpos;
	newpos.x = current_offset;
	newpos.y = line_below->get_area().pos.y;

	if (is_in_text(line_below, newpos))
		newpos.x = preferred_offset_x;

	set_caret_pos(get_position(newpos), false);
}

void TextBox::move_cursor_down()
{
	if (caret_pos.row == lines.size())
		return;

	float current_offset = cursor.pos.x;

	TextLine *line_above = get_line(caret_pos.row + 1);

	vec2 newpos;
	newpos.x = current_offset;
	newpos.y = line_above->get_area().pos.y;
	
	if (is_in_text(line_above, newpos))
		newpos.x = preferred_offset_x;

	set_caret_pos(get_position(newpos), false);
}

void TextBox::set_selection(const TextPosition &begin, const TextPosition &end)
{
	if (begin == end)
	{
		selecting = false;
		return;
	}
		
	selection_begin = begin;
	selection_end = end;
	selecting = true;
}

void TextBox::delete_selection()
{
	TextPosition start = selection_begin;
	TextPosition end = selection_end;

	if (start.row == selection_end.row)
		remove_at_index({ start.row, start.column + 1 }, end.column - start.column);
	else
	{
		String text = get_line(end.row)->get_text();
		String to_append = text.substr(end.column, text.size() - end.column);

		String start_text = get_line(start.row)->get_text();
		get_line(start.row)->set_text(start_text.substr(0, start.column) + to_append);

		for (int c = 0; c < end.row - start.row; c++)
			remove_line(start.row + 1);

		position_lines();
		check_slider_necessity();
	}


	stop_selecting();

	set_caret_pos(start);
}

void TextBox::set_selection_begin(const TextPosition & p_begin)
{
	selection_first = p_begin;
	update();
}

void TextBox::set_selection_end(const TextPosition & p_end)
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

void TextBox::stop_selecting()
{
	selecting = false;
	selection_begin = selection_end = selection_first = TextPosition();
}

void TextBox::make_visible(int p_index)
{
	rect2 item_area = get_line(p_index)->get_area();

	float delta = 0.0f;

	if (item_area.get_bottom() < area.get_bottom())
		delta = item_area.get_bottom() - area.get_bottom();

	if (item_area.get_top() > area.get_top())
		delta = item_area.get_top() - area.get_top();

	if (slider)
		slider->set_slider_pos(slider->get_value() - delta / extra_space);
}

void TextBox::slider_value_changed()
{
	scroll_offset = slider->get_value() * extra_space;

	position_lines();
}

void TextBox::give_color_to_word(const String& p_src, const Color& p_color)
{
	for (int i = 0; i < lines.size(); i++)
	{
		TextLine& l = lines[i];
		String s = l.get_text();
		Array<int> starts = s.find(p_src);
		Array<Color> colors;

		if (starts.size() < 1)
			continue;
		
		for (int c = 0; c < l.styles.size(); c++)
		{
			int end = l.get_text().size() - 1;
			if (c < l.styles.size() - 1)
				end = l.styles[c + 1].start - 1;

			for (int k = 0; k < end - l.styles[c].start; k++)
				colors.push_back(l.styles[c].color);
		}

		for (int c = 0; c < starts.size(); c++)
		{
			for (int k = starts[c]; k < starts[c] + p_src.size(); k++)
				colors[k] = p_color;
		}

		l.styles.clear();
		Color& last_color = colors[0];
		TextStyle t = { last_color, 0 };
		TextStyle& last_style = t;

		for (int c = 1; c < colors.size(); c++)
		{
			if (last_color != colors[c])
			{
				l.styles.push_back(last_style);
				last_color = colors[c];
				last_style = TextStyle({ last_color, c });
			}
		}

		l.styles.push_back(last_style);
	}
}

bool TextBox::is_in_text(TextLine *line, const vec2 &pos)
{
	return line->is_in_text(pos);
}

void TextBox::remove_at_index(const TextPosition& p_index, int p_length)
{
	TextLine* line = get_line(p_index.row);

	if (!line)
		return;

	if (p_index.column < 1)
	{
		if (p_index.row == 0)
			return;

		String text = line->get_text();
		remove_line(p_index.row);
		line = get_line(p_index.row - 1);
		
		int offset = line->get_text().size();

		line->set_text(line->get_text() + text);

		set_caret_pos(TextPosition(p_index.row - 1, offset + 1));

		position_lines();
		check_slider_necessity();

		return;
	}

	String text = line->get_text();
	text = text.erase(p_index.column - 1, p_length);

	line->set_text(text);
}

void TextBox::remove_line(int p_index)
{
	lines.clear(p_index);
}

void TextBox::insert_in_line(const TextPosition & p_pos)
{
	TextLine* l = get_line(p_pos.row);

	String text = l->get_text();

	String begin = text.substr(0, p_pos.column);
	String end = text.substr(p_pos.column, text.size() - p_pos.column);

	l->set_text(begin);

	insert_line_at_index(p_pos.row, TextLine(end));
	set_caret_pos(TextPosition(p_pos.row + 1, 0));
}

void TextBox::insert_at_index(const TextPosition& index, const String &text)
{
	TextLine* l = get_line(index.row);

	l->set_text(l->get_text().insert(index.column, text));
}

void TextBox::insert_at_selection(char c)
{
	insert_at_index(caret_pos, c);
	move_cursor_right();
}

void TextBox::insert_line_at_index(int index, TextLine& line)
{
	line.bind_textbox(this);

	if (index >= lines.size())
	{
		line.set_index(lines.size() - 1);
		push_back_line(line);
	}
	else
	{
		line.set_index(index);
		lines.insert(index + 1, line);
	}

	position_lines();
	check_slider_necessity();
}

#undef CLASSNAME
#define CLASSNAME TextBox

void TextBox::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(slider_value_changed);
	REG_METHOD(contextmenu_selected);
	REG_PROPERTY(text);
}