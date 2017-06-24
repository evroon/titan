#pragma once

#include "FBO.h"
#include "Math.h"
#include "world/World.h"
#include "math/Rect.h"
#include "world/Camera.h"
#include "ui/Canvas.h"
#include "core/Object.h"

class View;

class Viewport : public Object
{
	OBJ_DEFINITION(Viewport, Object);

public:
	Viewport();
	Viewport(View *_parentview);
	virtual ~Viewport();

	void init();

	void bind_parent(View *p_parentview);

	vec2 get_size() const;

	void resize(const rect2 &area);
	
	rect2 get_area(const vec2 &pos) const;
	vec2 get_screen_coords(const vec2 &pos) const;

	bool is_overlapping(const rect2 &area) const;
	bool is_in_box(const vec2 &pos) const;
	bool is_in_box(const rect2 &area) const;

	void activate_transform();

	void deactivate_transform();

	//use a custom world
	void set_world(World2D *_world);
	World2D* get_world() const;

	void set_canvas(Canvas *p_canvas);
	Canvas* get_canvas() const;

	void activate();
	void deactivate();

	void activate_world_transform();
	void deactivate_world_transform();

	void activate_canvas_transform();
	void deactivate_canvas_transform();

	void draw();
	void update();

	void handle_event(Event *event);

	Object* raycast(const vec2 &pos);

	static void bind_methods();

private:
	FBO2D *fbo;
	View *parentview;
	Camera *camera;

	World2D *world;
	Canvas *canvas;

	mat4 graphics_transform, input_transform;

	Viewport* return_viewport;

	rect2 renderarea;

	bool fps_locked;
};