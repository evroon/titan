#pragma once

#include "Control.h"
#include "graphics/Viewport.h"

class Model;

class WorldView : public Control
{
	OBJ_DEFINITION(WorldView, Control);

public:
	WorldView();
	WorldView(Scene* p_scene);
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

	enum DisplayMode
	{
		DISPLAY_WORLD,
		DISPLAY_CANVAS
	};

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void handle_event(UIEvent* ui_event) override;

	void post_draw_world();
	void post_draw_canvas();

	void update_camera();

	void set_postprocess(PostProcess* p_postprocess);
	PostProcess* get_postprocess() const;

	void set_scene(Scene* p_scene);
	Scene* get_scene() const;

	void select(Node* p_object);
	Node* get_selected() const;

	void highlight(Node* p_object);
	Node* get_highlight() const;

	void set_preview_type(int p_type);
	int get_preview_type();

	void set_transform_type(int p_transform_type);
	int get_transform_type() const;

	void set_simulating(bool p_simulating);
	bool get_simulating() const;

	void set_handle_2d(bool p_handle_2d);
	bool get_handle_2d() const;

	void set_display_mode(int p_display_mode);
	int get_display_mode() const;

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

	Node* selected;
	Node* highlighted;

	TransformType transform_type = TRANSLATE;
	DragType drag_type = DRAG_INACTIVE;
	DragType highlight_type = DRAG_INACTIVE;
	DisplayMode display_mode = DISPLAY_WORLD;

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

	bool handle_2d;

	bool simulating;

	int preview_type;
};