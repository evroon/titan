#pragma once

#include "Control.h"

#include "UICallback.h"
#include "Slider.h"
#include "SyntaxHighlighter.h"

class Label;
class TextBox;
class ContextMenu;

struct TextStyle
{
	Color color;
	int start;
};

struct TextLine
{
	TextLine(const String &p_text);
	~TextLine();

	void bind_textbox(TextBox *p_textbox);

	void set_index(int p_index);
	int get_index();

	rect2 get_area() const;
	void set_area(const rect2& p_area);

	bool is_in_text(const vec2& p_pos);

	String get_text() const;
	void set_text(const String& p_text);

	float get_height() const;

	int get_index(float x) const;
	float get_position_x(int index) const;

	Array<TextStyle> styles;

private:
	int index;
	TextBox *textbox;

	String text;

	rect2 area;
};

struct TextPosition
{
	int row, column;

	TextPosition() : TextPosition(-1, -1) { }
	TextPosition(int p_row, int p_column) { row = p_row; column = p_column; }
	
	bool operator== (const TextPosition &r) const { return row == r.row && column == r.column; }
	bool operator> (const TextPosition& p_r) const { return row > p_r.row || (row == p_r.row && column > p_r.column); }
};

class TextBox : public Control
{
	OBJ_DEFINITION(TextBox, Control);

public:
	TextBox();
	TextBox(const String& p_source);
	~TextBox();

	void switch_blink();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void position_lines();

	void notification(int p_notification) override;

	//unique methods
	void push_back_line(const String& p_source);
	void push_back_line(TextLine line);

	void set_font(Font* p_font);
	Font* get_font() const;

	void clear_text();
	String get_text() const;
	void set_text(const String &source);

	void contextmenu_selected(int p_index);

	void cut_text();
	void copy_text();
	void paste_text();

	String get_selected_text();

	TextPosition get_position(const vec2 &pos);

	TextLine* get_line(int row);
	int get_line_size(int row);

	void set_caret_pos(const TextPosition &p_pos, bool update_preferred = true);
	void set_caret_bottom();

	void update_caret();

	void set_line_numbers_enabled(bool p_value);

	void check_slider_necessity();

	void add_slider();
	void remove_slider();

	void move_cursor_begin();
	void move_cursor_end();
	void move_cursor_left();
	void move_cursor_right();
	void move_cursor_up();
	void move_cursor_down();

	bool is_in_text(TextLine *line, const vec2 &pos);

	void remove_at_index(const TextPosition& p_index, int p_length = 1);
	void remove_line(int p_index);

	void insert_in_line(const TextPosition &p_pos);

	void insert_at_index(const TextPosition &index, const String &text);
	void insert_at_selection(char c);
	void insert_line_at_index(int index, TextLine line);

	void set_selection(const TextPosition &begin, const TextPosition &end);
	void delete_selection();
	void set_selection_begin(const TextPosition &p_begin);
	void set_selection_end(const TextPosition &p_end);
	void stop_selecting();

	void make_visible(int p_index);

	void slider_value_changed();

	void give_color_to_word(TextLine& p_line, const String& p_src, const Color& p_color);
	void give_color_to_patterns(TextLine& p_line);

	void handle_extension(const String& p_extension);
	void set_language(SyntaxHighlighter::Language p_language);
	void update_highlighting();
	void update_highlighting(TextLine& p_line);

	static void bind_methods();

private:
	void set_colors_on_line(TextLine& p_line, const Array<int>& p_starts, const Array<String>& p_matches, const Color& p_color);

	rect2 box, cursor;
	TextPosition caret_pos;

	float top_margin;
	float left_margin;
	float right_margin;

	Array<TextLine> lines;

	Font* font;
	Slider* slider;

	TextPosition selection_begin;
	TextPosition selection_end;
	TextPosition selection_first;
	bool selecting;

	float scroll_offset;

	// The space in y direction not drawn on screen (if scrolling)
	float extra_space;

	// Used when pressing up/down button
	float preferred_offset_x;

	ContextMenu* menu;

	UITimer *ibeam_blinker;
	bool ibeam_blinks;

	bool line_numbers_enabled;

	Color background_color;
	Color cursor_color;
	Color line_numbers_color;
	Color selection_color;

	SyntaxHighlighter::Language language;
};

