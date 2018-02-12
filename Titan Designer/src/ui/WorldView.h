#pragma once

#include "Control.h"
#include "graphics/Viewport.h"

class Model;

class WorldView : public Control
{
	OBJ_DEFINITION(WorldView, Control);

public:
	WorldView();
	WorldView(World * p_world);
	virtual ~WorldView();

	enum DragType
	{
		DRAG_INACTIVE,
		DRAG_X,
		DRAG_Y,
		DRAG_Z
	};

	enum TransformType
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

	enum DrawMode
	{
		DEFAULT,
		WIREFRAME
	};

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void handle_event(UIEvent* ui_event) override;

	void post_draw_world();

	void set_postprocess(PostProcess* p_postprocess);
	PostProcess* get_postprocess() const;

	void set_world(World* p_world);
	World* get_world();

	void select(WorldObject* p_object);
	WorldObject* get_selected() const;

	void highlight(WorldObject* p_object);
	WorldObject* get_highlight() const;

	void set_preview_type(int p_type);
	int get_preview_type();

	void set_transform_type(int p_transform_type);
	int get_transform_type() const;

	WorldObject* raycast(const vec2& p_pos) const;

	Viewport* get_viewport() const;

	static void bind_methods();

private:
	void draw_grid();

	FBO2D* raycast_fbo;
	SimpleMesh* plane;
	Shader* raycast_shader;

	EditorViewport* viewport;
	PostProcess* postprocess;
	Viewport* return_viewport;
	Texture2D* preview_texture;

	WorldObject* selected;
	WorldObject* highlighted;

	TransformType transform_type = TRANSLATE;
	DragType drag_type = DRAG_INACTIVE;
	DragType highlight_type = DRAG_INACTIVE;
	vec3 drag_start_pos;
	vec3 drag_start_size;
	vec3 drag_start_rotation;
	vec3 drag_start_grab;

	bool item_dragging;
	vec2 prev_item_drag_pos;
	
	bool cam_dragging;
	vec2 prev_cam_drag_pos;

	Model* cone;
	Model* disk;

	bool handle_2d = false;

	int preview_type;
};