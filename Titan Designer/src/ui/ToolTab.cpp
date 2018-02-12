#include "ToolTab.h"

#include "world/Terrain.h"

#include "PropertyView.h"

ToolTab::ToolTab() : ToolTab(NULL)
{
}

ToolTab::ToolTab(Terrain* p_terrain)
{
	terrain = p_terrain;

	property_view = new PropertyView;
	property_view->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_END);
	property_view->set_margins(4, 4, 4, 4);

	add_child(property_view);

	if (terrain)
		property_view->set_property(terrain->get_brush());

	set_tab_title("Tools");
}

vec2 ToolTab::get_required_size() const
{
	return vec2(100.0f, 100.0f);
}

#undef CLASSNAME
#define CLASSNAME ToolTab

void ToolTab::bind_methods()
{
	
}
