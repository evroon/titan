#pragma once

#include "Control.h"
#include "graphics/Viewport.h"

class WorldView : public Control
{
	OBJ_DEFINITION(WorldView, Control);

public:
	WorldView();
	WorldView(World * p_world);
	virtual ~WorldView();

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void handle_event(UIEvent* ui_event) override;

	void set_postprocess(PostProcess* p_postprocess);
	PostProcess* get_postprocess() const;

	void set_world(World* p_world);
	World* get_world();

	void select(WorldObject* p_object);
	WorldObject* get_selected() const;

	void highlight(WorldObject* p_object);
	WorldObject* get_highlight() const;

	static void bind_methods();

private:
	void draw_grid();

	Viewport* viewport;
	PostProcess* postprocess;

	WorldObject* selected;
	WorldObject* highlighted;

	bool item_dragging;
	vec2 prev_item_drag_pos;
	
	bool cam_dragging;
	vec2 prev_cam_drag_pos;
};