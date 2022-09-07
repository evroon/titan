#pragma once

#include "container.h"
#include "uibox.h"
#include "labelbutton.h"
#include "contextmenu.h"



class ToolBar : public Control
{
	OBJ_DEFINITION(ToolBar, Control);

public:
	struct ToolBarItem
	{
		ContextMenu *menu;
		rect2 area;
		String text;
	};

	ToolBar();
	virtual ~ToolBar();

	vec2 get_required_size() const override;
	void notification(int p_notification) override;

	void handle_event(UIEvent* p_ui_event) override;

	void position_items();

	int get_item(const vec2& p_pos) const;

	void add_item(const String& p_text);
	void set_menu(int p_index, ContextMenu* p_menu);

	void open_item(int p_index);

	static void bind_methods();

private:
	void menu_closed();

	Font* font;

	Array<ToolBarItem> items;

	float left_margin;
	float right_margin;
	float seperation;

	int highlighted;
	int selected;

	Color selection_color;
	Color highlight_color;

};