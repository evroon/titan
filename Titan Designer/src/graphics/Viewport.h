#pragma once

#include "FBO.h"
#include "math/Math.h"
#include "world/World.h"
#include "math/Rect.h"
#include "world/Camera.h"
#include "ui/Canvas.h"
#include "core/Object.h"
#include "graphics/PostProcess.h"
#include "game/Scene.h"

class View;
class Renderer;
class WorldView;

class Viewport : public Node
{
	OBJ_DEFINITION(Viewport, Node);

public:
	Viewport();
	Viewport(Renderer* p_renderer);
	virtual ~Viewport();

	friend class Renderer;
	friend class ForwardRenderer;
	friend class DeferredRenderer;

	enum DrawDestination
	{
		// draw directly to the screen
		DIRECT,

		// draw to a framebuffer
		FRAMEBUFFER,

		// draw to a framebuffer and then to the screen
		POSTPROCESS
	};

	enum DrawMode
	{
		FORWARD_RENDERING,
		DEFERRED_RENDERING
	};

	void init();

	void bind_parent(View *p_parentview);

	vec2 get_size() const;

	void resize(const rect2 &p_area);
	
	rect2 get_area() const;
	vec2 get_screen_coords(const vec2 &p_pos) const;

	bool is_overlapping(const rect2 &p_area) const;
	bool is_in_box(const vec2 &p_pos) const;
	bool is_in_box(const rect2 &p_area) const;

	void set_renderer(Renderer* p_renderer);
	Renderer* get_renderer() const;
	
	void set_scene(Scene* p_scene);
	Scene* get_scene() const;

	//use a custom world
	void set_world(World *p_world);
	World* get_world() const;

	void set_canvas(Canvas *p_canvas);
	Canvas* get_canvas() const;

	//draw to a framebuffer instead of directly to the window
	void set_mode(DrawDestination p_mode);
	int get_mode() const;

	void set_wireframe_enabled(bool p_wireframe_enabled);
	bool get_wireframe_enabled() const;

	void set_fbo(FBO2D *p_fbo);
	FBO2D* get_fbo() const;

	void set_postprocess(PostProcess* p_postprocess);
	PostProcess* get_postprocess() const;

	int get_delta_time() const;

	void activate();
	void deactivate();

	mat4 get_input_transform() const;
	
	virtual void post_draw_world();
	virtual void post_draw_canvas();
	void draw();

	void update();

	void handle_event(Event* p_event);

	vec3 deferred_raycast(const vec2& p_pos) const;

	Object* raycast(const vec2& p_pos) const;

	static void bind_methods();

private:
	DrawDestination destination;
	DrawMode mode;

	FBO2D *fbo;
	PostProcess *postprocess;

	View *parentview;
	Camera *camera;

	Scene* scene;
	World* world;
	Canvas* canvas;

	mat4 graphics_transform;
	mat4 input_transform;

	RenderTarget limiter;
	FPSLimiter updatelim;

	float delta_time;
	float fps;
	float fps_guess;

	Renderer* renderer;

	Viewport *return_viewport;

	rect2 renderarea;

	bool fps_locked;
	bool wireframe_enabled;
};

class EditorViewport : public Viewport
{
	OBJ_DEFINITION(EditorViewport, Viewport);

public:
	EditorViewport(Renderer* p_renderer);

	void post_draw_world() override;
	void post_draw_canvas() override;

	WorldView* worldview;
};