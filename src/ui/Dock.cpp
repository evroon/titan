#include "Dock.h"

#include "PropertyTab.h"
#include "EditorCanvas.h"

#include "Memory.h"

//=========================================================================
//Dock
//=========================================================================

Dock::Dock()
{
	highlight_color = DEFAULT_THEME->get_highlight_color();
	selected_color = DEFAULT_THEME->get_selection_color();
	inactive_color = TO_RGB(50);

	left_margin = 6.0f;
	text_offset = 4.0f;
}

vec2 Dock::get_required_size() const
{
	return vec2(150);
}

void Dock::handle_event(UIEvent * p_event)
{
	switch (p_event->type)
	{
	case UIEvent::FOCUS_LOSE:

		highlighted = -1;

		break;
	case UIEvent::MOUSE_EXIT:

		highlighted = -1;

		break;

	case UIEvent::MOUSE_HOVER:

		if (!area.is_in_box(p_event->pos))
		{
			selected = -1;
			highlighted = -1;
		}

		if (selected != -1)
			break;

		highlighted = get_item(p_event->pos);

		break;

	case UIEvent::MOUSE_PRESS:

		if (p_event->button_type == Mouse::CENTER)
		{
			remove_tab(get_item(p_event->pos));


		}

		if (p_event->press_type == Event::DOWN)
		{
			selected = get_item(p_event->pos);

			set_active_tab(selected);

			highlighted = -1;
		}
		else
		{
			selected = -1;
			highlighted = get_item(p_event->pos);
		}

		break;
	}
}

void Dock::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		draw_frame(tab_area, TO_RGB(90));

		for (int c = 0; c < selectors.size(); c++)
		{
			vec2 pos = vec2(selectors[c].area.get_left(), selectors[c].area.pos.y);

			Color color = inactive_color;

			if (active == c)
				color = TO_RGB(90);

			if (selected == c)
				color = selected_color;

			else if (highlighted == c)
				color = highlight_color;

			draw_box(selectors[c].area, color);
			draw_text(selectors[c].name, pos + vec2(text_offset, 0.0f));

			if (active != -1)
			{
				tabs[active]->draw();
			}
		}

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		selectors_area = rect2(area.get_left(), area.get_right(), area.get_top(), area.get_top() - 30.0f);
		tab_area = rect2(area.get_left(), area.get_right(), area.get_top() - 30.0f, area.get_bottom());
		
		position_items();

		break;
	}
}

int Dock::get_item(const vec2& p_pos) const
{
	int result = -1;

	for (int c = 0; c < selectors.size(); c++)
	{
		if (selectors[c].area.is_in_box(p_pos))
		{
			result = c;
			break;
		}
	}

	return result;
}

void Dock::position_items()
{
	float offset_x = area.get_left() + left_margin;

	for (int c = 0; c < selectors.size(); c++)
	{
		selectors[c].name = tabs[c]->get_tab_title();

		float width = MAX(get_default_font()->get_width(selectors[c].name) + text_offset * 2.0f, 100);

		selectors[c].area = rect2(offset_x, offset_x + width, selectors_area.get_top(), selectors_area.get_bottom());

		offset_x += width;
	}
}

void Dock::add_tab(Tab *p_tab)
{
	Selector sel;
	sel.name = p_tab->get_tab_title();

	tabs.push_back(p_tab);
	selectors.push_back(sel);

	p_tab->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	p_tab->set_margins(0, 0, 0, 30);

	set_active_tab(selectors.size() - 1);

	position_items();
}

void Dock::remove_tab(int p_index)
{
	set_active_tab(0);

	GC->queue_clean(tabs[p_index]);

	tabs.clear(p_index);
	selectors.clear(p_index);

	position_items();
}

void Dock::set_active_tab(int p_index)
{
	if (p_index == -1)
		return;

	active = p_index;

	children.clear();

	add_child(tabs[active]);
}