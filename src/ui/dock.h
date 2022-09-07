#pragma once

#include "tab.h"
#include "control.h"
#include "button.h"
#include "uibox.h"

class Dock : public Control
{
	OBJ_DEFINITION(Dock, Control);

public:

	struct Selector
	{
		String name;
		rect2 area;
		bool is_adder;
	};

	Dock();

	vec2 get_required_size() const override;

	void handle_event(UIEvent * p_event) override;

	void notification(int p_notification) override;

	int get_item(const vec2 & p_pos) const;

	void position_items();

	void add_tab(Tab *tab);
	void remove_tab(int p_index);
	void set_active_tab(int p_index);

private:
	Vector<Tab> tabs;

	Array<Selector> selectors;

	int selected = -1;
	int highlighted = -1;
	int active = -1;

	float left_margin;
	float text_offset;

	rect2 tab_area, selectors_area;

	Color selected_color;
	Color highlight_color;
	Color inactive_color;
};
