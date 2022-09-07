#include "tab.h"

#include "dock.h"

Tab::Tab()
{
	title = "";
}


Tab::~Tab()
{
}

void Tab::set_tab_title(const String& p_title)
{
	title = p_title;

	if (parent && parent->derives_from_type<Dock*>())
		parent->cast_to_type<Dock*>()->position_items();
}

String Tab::get_tab_title() const
{
	return title;
}

vec2 Tab::get_required_size() const
{
	return vec2(400);
}