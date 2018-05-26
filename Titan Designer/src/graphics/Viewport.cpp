#include "Viewport.h"

#include "game/Scene.h"
#include "Renderer.h"

#include "View.h"

#include "core/WindowManager.h"

#include "ui/worldview.h"

Viewport::Viewport()
{
	VIEW->bind_viewport(this);
	fbo = NULL;
}


Viewport::Viewport(Renderer * p_renderer) : Viewport()
{
	set_renderer(p_renderer);
}

Viewport::~Viewport()
{
	delete canvas;
}

void Viewport::set_world(World* p_world)
{
	if (world)
		remove_child(world);

	world = p_world;
	add_child(world);
}

World* Viewport::get_world() const
{
	return world;
}

void Viewport::set_canvas(Canvas* p_canvas)
{
	if (canvas)
		remove_child(canvas);

	canvas = p_canvas;
	add_child(canvas);
	canvas->resize();
}

Canvas* Viewport::get_canvas() const
{
	return canvas;
}

void Viewport::set_mode(DrawDestination p_mode)
{
	destination = p_mode;
}

int Viewport::get_mode() const
{
	return destination;
}

void Viewport::set_fbo(FBO2D *p_fbo)
{
	fbo = p_fbo;
}

FBO2D* Viewport::get_fbo() const
{
	return fbo;
}

void Viewport::set_postprocess(PostProcess* p_postprocess)
{
	postprocess = p_postprocess;
	destination = POSTPROCESS;
}

PostProcess* Viewport::get_postprocess() const
{
	return postprocess;
}

int Viewport::get_delta_time() const
{
	return 0;
}

void Viewport::init()
{
	if (canvas)
		canvas->init(this);

	if (world)
		world->init();
}

void Viewport::bind_parent(View *p_parentview)
{
	parentview = p_parentview;
}

vec2 Viewport::get_size() const
{
	return renderarea.size;
}

void Viewport::resize(const rect2 &p_area)
{
	renderarea = p_area;

	vec2 size = p_area.size;
	vec2 window_size = WINDOWSIZE_F;

	mat4 translate, rotate, scale;

	translate.translate(vec3(renderarea.pos.x / size.x, renderarea.pos.y / size.y, 0));
	scale.scale(vec3(1.0f / size.x, 1.0f / size.y, 0.0f));

	graphics_transform = scale;

	vec3 s = vec3(size * 2.0f / window_size, 0.0f);
	//input_transform = mat4::Scale(vec3(size / window_size, 0));
	input_transform = mat4::Translate(vec3(-p_area.pos.x, -p_area.pos.y, 0));

	if (canvas)
	{
		canvas->resize();
	}

	if (renderer)
		renderer->resized();
}

rect2 Viewport::get_area() const
{
	return renderarea;
}

vec2 Viewport::get_screen_coords(const vec2 &p_pos) const
{
	if (!world)
		return vec2();

	vec4 p = { (p_pos - renderarea.pos) / get_size(), 0.0f, 1.0f };

	Camera* cam = world->get_active_camera();
	mat4 inv = cam->get_inverse();

	return (inv * p).get_xy();
}

bool Viewport::is_overlapping(const rect2 &area) const
{
	return renderarea.is_overlapping(area);
}

bool Viewport::is_in_box(const vec2 &pos) const
{
	return renderarea.is_in_box(pos);
}

bool Viewport::is_in_box(const rect2 &area) const
{
	return renderarea.is_in_box(area);
}

void Viewport::set_renderer(Renderer* p_renderer)
{
	renderer = p_renderer;
	renderer->set_viewport(this);
	renderer->activate();
}

Renderer* Viewport::get_renderer() const
{
	return renderer;
}

void Viewport::set_scene(Scene* p_scene)
{
	scene = p_scene;
	set_world(scene->get_child_by_type<World*>());
	set_canvas(scene->get_child_by_type<Canvas*>());
}

Scene* Viewport::get_scene() const
{
	return scene;
}

void Viewport::activate()
{
	return_viewport = VIEW->get_active_viewport();
	VIEW->set_active_viewport(this);
	renderer->activate();
}

void Viewport::deactivate()
{
	if (!return_viewport)
		return;

	return_viewport->activate();
	return_viewport = NULL;
}

mat4 Viewport::get_input_transform() const
{
	return input_transform;
}

void Viewport::post_draw_world()
{

}

void Viewport::post_draw_canvas()
{

}

void Viewport::draw()
{
	bool s = limiter.should_update();
	fps = to_float(limiter.fps_measured);
	fps_guess = to_float(limiter.fps_guessed);

	if (s && renderer)
	{
		activate();
		renderer->render();
		deactivate();
	}
}

void Viewport::update()
{
	//Handle Update Loop
	FPSLimiter::FPSInfo i;
	i = updatelim.update();
	delta_time = i.delta_time;

	if (i.needsupdate)
	{
		activate();

		if (world)
			world->update();

		if (canvas)
			canvas->update();

		deactivate();
	}
}

void Viewport::handle_event(Event *event)
{
	//activate_transform();

	if (world)
		world->handle_event(event);

	if (canvas)
		canvas->handle_event(event);

	//deactivate_transform();
}

vec3 Viewport::deferred_raycast(const vec2 & p_pos) const
{
	return DEFERRED_RENDERER->get_position_at_pixel(p_pos);
}

Object* Viewport::raycast(const vec2 &pos) const
{
	vec2 projected = get_screen_coords(pos);

	if (world)
	{
		for (int c = 0; c < world->get_child_count(); c++)
		{
			WorldObject* obj = world->get_child(c)->cast_to_type<WorldObject*>();

			if (obj->is_of_type<Camera>())
				continue;

			if (obj->CheckOverlap(projected))
				return world->get_child(c);
		}
	}

	return NULL;
}

#undef CLASSNAME
#define CLASSNAME Viewport

void Viewport::bind_methods()
{
	REG_PROPERTY(world);
	REG_PROPERTY(canvas);
}

EditorViewport::EditorViewport(Renderer * p_renderer) : Viewport(p_renderer)
{
}

void EditorViewport::post_draw_world()
{
	worldview->post_draw_world();
}

void EditorViewport::post_draw_canvas()
{
	worldview->post_draw_canvas();
}
