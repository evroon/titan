#pragma once

#include "FBO.h"
#include "Math.h"
#include "world/World.h"
#include "math/Rect.h"
#include "world/Camera.h"
#include "ui/Canvas.h"
#include "core/Object.h"
#include "graphics/VisualEffect.h"

class View;

class Viewport : public Object
{
	OBJ_DEFINITION(Viewport, Object);

public:
	Viewport();
	Viewport(View *_parentview);
	virtual ~Viewport();

	enum DrawMode
	{
		// draw directly to the screen
		DIRECT,

		// draw to a framebuffer
		FRAMEBUFFER,

		// draw to a framebuffer and then to the screen
		POSTPROCESS
	};

	void init();

	void bind_parent(View *p_parentview);

	vec2 get_size() const;

	void resize(const rect2 &p_area);
	
	rect2 get_area(const vec2 &p_pos) const;
	vec2 get_screen_coords(const vec2 &p_pos) const;

	bool is_overlapping(const rect2 &p_area) const;
	bool is_in_box(const vec2 &p_pos) const;
	bool is_in_box(const rect2 &p_area) const;

	void activate_transform();

	void deactivate_transform();

	//use a custom world
	void set_world(World *p_world);
	World* get_world() const;

	void set_canvas(Canvas *p_canvas);
	Canvas* get_canvas() const;

	//draw to a framebuffer instead of directly to the window
	void set_mode(DrawMode p_mode);
	int get_mode() const;

	void set_fbo(FBO2D *p_fbo);
	FBO* get_fbo() const;

	void set_postprocess(PostProcess* p_postprocess);
	PostProcess* get_postprocess() const;

	void activate();
	void deactivate();

	void activate_world_transform();
	void deactivate_world_transform();

	void activate_canvas_transform();
	void deactivate_canvas_transform();

	void draw();
	void update();

	void handle_event(Event* p_event);

	Object* raycast(const vec2& p_pos);

	static void bind_methods();

private:
	DrawMode mode;

	FBO2D *fbo;
	PostProcess *postprocess;

	View *parentview;
	Camera *camera;

	World *world;
	Canvas *canvas;

	mat4 graphics_transform;
	mat4 input_transform;

	Viewport *return_viewport;

	rect2 renderarea;

	bool fps_locked;
};