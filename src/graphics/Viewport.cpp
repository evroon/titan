#include "Viewport.h"

#include "game/Scene.h"
#include "Renderer.h"

#include "View.h"

#include "core/WindowManager.h"

Viewport::Viewport()
{
	camera = new Camera;
	fbo = NULL;
}

Viewport::Viewport(View *p_parentview) : Viewport()
{
	bind_parent(p_parentview);
}

Viewport::~Viewport()
{
	delete canvas;
}

void Viewport::set_world(World* p_world)
{
	world = p_world;
}

World* Viewport::get_world() const
{
	return world;
}

void Viewport::set_canvas(Canvas* p_canvas)
{
	canvas = p_canvas;
}

Canvas* Viewport::get_canvas() const
{
	return canvas;
}

void Viewport::set_mode(DrawMode p_mode)
{
	mode = p_mode;
}

int Viewport::get_mode() const
{
	return mode;
}

void Viewport::set_fbo(FBO2D *p_fbo)
{
	fbo = p_fbo;
}

FBO* Viewport::get_fbo() const
{
	return fbo;
}

void Viewport::set_postprocess(PostProcess* p_postprocess)
{
	postprocess = p_postprocess;
	mode = POSTPROCESS;
}

PostProcess* Viewport::get_postprocess() const
{
	return postprocess;
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
	vec2 window_size = vec2(to_float(WINDOWSIZE.x), to_float(WINDOWSIZE.y));

	mat4 translate, rotate, scale;

	translate.translate(vec3(renderarea.pos.x / size.x, renderarea.pos.y / size.y, 0));
	scale.scale(vec3(1.0f / size.x, 1.0f / size.y, 0.0f));

	graphics_transform = scale;

	vec3 s = vec3(size * 2.0f / window_size, 0.0f);
	input_transform = mat4::Translate(vec3(-p_area.pos.x, -p_area.pos.y, 0));

	if (canvas)
		canvas->resize();
}

rect2 Viewport::get_area(const vec2 &p_pos) const
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

void Viewport::activate_transform()
{
	return_viewport = VIEW->get_active_viewport();
	
	VIEW->set_active_viewport(this);
	
	vec2 bottom_left = vec2(to_float(WINDOWSIZE.x), to_float(WINDOWSIZE.y)) / vec2(2.0f);

	glViewport(
		to_int(bottom_left.x + renderarea.get_left()),
		to_int(bottom_left.y + renderarea.get_bottom()),
		to_int(renderarea.size.x * 2.0f),
		to_int(renderarea.size.y * 2.0f));
}

void Viewport::deactivate_transform()
{
	VIEW->set_active_viewport(return_viewport);

	return_viewport = NULL;

	glViewport(0, 0, to_int(WINDOWSIZE.x), to_int(WINDOWSIZE.y));
}

void Viewport::activate()
{
	VIEW->set_active_viewport(this);

	return_viewport = VIEW->get_active_viewport();
}

void Viewport::deactivate()
{
	VIEW->set_active_viewport(return_viewport);

	return_viewport = NULL;
}

void Viewport::activate_world_transform()
{
	RENDERER->switch_to_camera(world->get_active_camera());
}

void Viewport::deactivate_world_transform()
{
	RENDERER->deactivate_camera();
}

void Viewport::activate_canvas_transform()
{
	RENDERER->activate_projection_matrix(graphics_transform);
	RENDERER->activate_view_matrix(mat4());
}

void Viewport::deactivate_canvas_transform()
{
	RENDERER->deactivate_projection_matrix();
	RENDERER->deactivate_view_matrix();
}

void Viewport::draw()
{
	FBOMANAGER->bind_default_fbo();

	if ((mode == FRAMEBUFFER || mode == POSTPROCESS) && fbo)
		fbo->bind();

	//activate_transform();
	
	if (world)
	{
		if (world->get_active_camera())
			world->get_active_camera()->update();

		//activate_world_transform();
		world->draw();
		//deactivate_world_transform();
	}

	if (canvas)
	{
		activate_canvas_transform();
		canvas->draw();
		deactivate_canvas_transform();
	}

	//deactivate_transform();

	if (mode == POSTPROCESS && postprocess)
		postprocess->post_process();

	if (fbo)
		fbo->unbind();
}

void Viewport::update()
{
	activate();

	if (world)
		world->update();

	if (canvas)
		canvas->update();

	deactivate();
}

void Viewport::handle_event(Event *event)
{
	activate_transform();

	if (world)
		world->handle_event(event);

	if (canvas)
		canvas->handle_event(event);

	deactivate_transform();
}

Object* Viewport::raycast(const vec2 &pos)
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
