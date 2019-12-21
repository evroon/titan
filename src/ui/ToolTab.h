#pragma once

#include "Tab.h"

class PropertyView;
class Terrain;

class ToolTab : public Tab
{
	OBJ_DEFINITION(ToolTab, Tab);

public:
	ToolTab();
	ToolTab(Terrain* p_terrain);

	vec2 get_required_size() const override;


	static void bind_methods();

private:
	PropertyView* property_view;

	Terrain* terrain;
};