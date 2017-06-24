#pragma once

#include "Tab.h"
#include "WorldView.h"
#include "Container.h"

class GamePreviewTab : public Tab
{
	OBJ_DEFINITION(GamePreviewTab, Tab);

public:
	GamePreviewTab();
	GamePreviewTab(World2D * p_world);
	virtual ~GamePreviewTab();
	
	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void world_changed();

	WorldView* get_world_view() const;
	
private:
	Container buttons;
	WorldView* world_view;
};

