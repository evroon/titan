#pragma once

#include "Tab.h"
#include "WorldView.h"
#include "Container.h"
#include "ComboBox.h"

class GamePreviewTab : public Tab
{
	OBJ_DEFINITION(GamePreviewTab, Tab);

public:
	GamePreviewTab();
	GamePreviewTab(Scene* p_scene);
	virtual ~GamePreviewTab();
	
	vec2 get_required_size() const override;

	void update_textures();

	void transform_toggled(int p_index);

	void set_preview_type(const String& p_type);

	void notification(int p_notification) override;

	static void bind_methods();

	void world_changed();

	WorldView* get_world_view() const;
	
private:
	Container buttons;
	WorldView* world_view;
	ComboBox* combo_box;
	ComboBox* draw_mode;
};

