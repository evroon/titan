#pragma once

#include "control.h"
#include "uicallback.h"

class Slider;
class TreeView;
class ContextMenu;

class TreeElement : public Object
{
	OBJ_DEFINITION(TreeElement, Object);

public:
	friend class TreeView;

	TreeElement(const String &p_text);
	TreeElement(TreeView* p_treeview);
	~TreeElement();

	void push_child(TreeElement* p_child);
	void remove_child(TreeElement* p_child);
	void clear();

	void bind_treeview(TreeView *p_textbox);

	void update_init_expanded();

	void set_index(int p_index);
	int get_index();

	rect2 get_area() const;
	void set_area(const rect2& p_area);

	bool is_in_area(const vec2& p_pos);

	String get_text() const;
	void set_text(const String& p_text);

	Texture2D* get_icon() const;
	void set_icon(Texture2D* p_icon);

	void set_visible(bool p_visible);
	bool get_visible() const;

	float get_height() const;
	float get_item_height() const;

	int get_index(float x) const;
	float get_position_x(int index) const;

	void set_expanded(bool p_expanded);
	bool get_expanded() const;

	bool search(const String& p_text);
	void clear_search();

	TreeElement* get_item(const String& p_name);

	//returns new bottom
	float position(float p_top);

	bool is_selected() const;

	TreeElement* get_parent() const;
	TreeElement* get_next() const;
	TreeElement* get_prev();

	TreeElement* get_next_visible();
	TreeElement* get_prev_visible();


private:
	bool selected;

	int index = 0;
	int depth = 0;

	TreeElement* parent;
	TreeElement* next;
	TreeView* treeview;

	TreeElement* childs;

	Texture2D* icon;

	String text;
	String secondary_text;

	rect2 area;

	bool visible;
	bool expanded;
};

class TreeView : public Control
{
	OBJ_DEFINITION(TreeView, Control);

public:

	friend class TreeElement;

	TreeView();
	virtual ~TreeView();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void position_items();

	void check_slider_necessity();

	void notification(int p_notification) override;

	TreeElement* create_item(TreeElement* p_parent);

	void push_node(Node* p_node);
	TreeElement* create_item_from_node(Node* p_node, TreeElement* p_parent);

	void clear();
	void push_back_path(const String & p_path);
	void remove_item(TreeElement* p_item);

	TreeElement* get_item(const vec2& p_pos);
	TreeElement* get_item(const String& p_name);

	void set_item_numbers_enabled(bool p_value);

	void add_slider();
	void remove_slider();

	void set_selected(TreeElement* p_item);
	TreeElement* get_selected() const;

	void move_selected_begin();
	void move_selected_end();
	void move_selected_up();
	void move_selected_down();

	void select_none();

	void make_visible(TreeElement* p_item);

	void set_init_expanded(bool p_init_expanded);

	// Set the depth until which the nodes are expanded initially.
	void set_init_expanded_depth(int p_init_expanded_depth);
	int get_init_expanded_depth() const;

	void search(const String& p_src);
	void stop_search();

	void slider_value_changed();

	void set_font(Font* p_font);
	Font* get_font() const;

	static void bind_methods();

private:
	TreeElement* highlighted;
	TreeElement* selected;

	float scroll_offset;

	//the space in y direction not drawn on screen (if scrolling)
	float extra_space;

	bool item_numbers_enabled;

	Color background_color;
	Color selection_color;
	Color item_numbers_color;

	rect2 box;

	String search_text;

	float top_margin;
	float left_margin;
	float right_margin;
	float internal_left_margin;
	float secondary_margin;

	Vector<TreeElement> roots;

	Font* font;
	Slider* slider;

	rect2 selection_box;

	float depth_offset;

	int init_expanded_depth;

	Ref<Texture2D> collapsed_tex;
	Ref<Texture2D> expanded_tex;
};