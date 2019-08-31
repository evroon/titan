#include "WorldView.h"

#include "graphics/Renderer.h"
#include "Core/WindowManager.h"
#include "graphics/View.h"

#include "world/Raycaster.h"
#include "world/Model.h"
#include "world/Terrain.h"

WorldView::WorldView() : WorldView(NULL)
{

}

WorldView::WorldView(Scene* p_scene)
{
	Renderer* r = new DeferredRenderer;
	r->set_draw_on_screen(false);

	viewport = new EditorViewport(r);
	viewport->worldview = this;

	FBO2D* fbo = new FBO2D(WINDOWSIZE);
	fbo->cleared_every_frame = false;

	viewport->set_scene(p_scene);
	viewport->set_mode(Viewport::FRAMEBUFFER);
	viewport->set_fbo(fbo);

	fbo->add_color_texture();
	fbo->add_depth_texture();
	fbo->init();

	preview_texture = r->get_texture(DeferredRenderer::FINAL_COLOR)->cast_to_type<Texture2D*>();

	selected = NULL;

	cone = new Model("assets/Models/Primitives/Cone.dae");
	plane = MeshHandler::get_singleton()->get_plane();
	disk = new Model("assets/Models/Primitives/Cube.dae");

	raycast_fbo = new FBO2D(WINDOWSIZE);
	raycast_fbo->add_float_color_texture();
	raycast_fbo->init();

	raycast_shader = CONTENT->LoadShader("EngineCore/Shaders/RaycastPlane");

	set_update_continuoulsy(true);

	handle_2d = false;

}

WorldView::~WorldView()
{
}

vec2 WorldView::get_required_size() const
{
	return vec2(200);
}

void WorldView::notification(int p_notification)
{
	if (!viewport)
		return;

	World* world = viewport->get_world();
	Canvas* canvas = viewport->get_canvas();

	
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		if (viewport)
			viewport->init();
		
		break;

	case NOTIFICATION_UPDATE:
		break;

	case NOTIFICATION_DRAW:

		return_viewport = ACTIVE_VIEWPORT;
		viewport->get_fbo()->clear();

		if (simulating)
			viewport->update();

		viewport->draw();

		RENDERER->use_scissor(area);

		postprocess = false;

		if (!postprocess)
			draw_texture(preview_texture, get_area(), Color::White, vec4(0, 1, 0, 1));
		else
			postprocess->post_process();

		RENDERER->stop_scissor();

		/*if (handle_2d)
		{
			for (int c = 0; c < canvas->get_child_count(); c++)
			{
				Control* object = canvas->get_child_by_index(c)->cast_to_type<Control*>();

				if (object->is_of_type<Camera>())
					continue;

				vec2 size = object->get_size();
				vec2 pos = object->get_pos();

				DrawCommand command;
				command.type = DrawCommand::FRAME;
				command.area = rect2(pos, size);
				command.color = TO_RGB(vec3i(255, 164, 66));
				command.tex = CanvasData::get_singleton()->get_default_theme()->get_highlight();				

				if (highlighted == object)
					render_frame(command);

				if (highlighted == object)
					render_frame(command);
			}
			for (int c = 0; c < world->get_child_count(); c++)
			{
				WorldObject* object = world->get_child_by_index(c)->cast_to_type<WorldObject*>();

				if (!object || object->is_of_type<Camera>())
					continue;

				vec2 size = object->get_size().get_xy();
				vec2 pos = object->get_pos().get_xy();

				if (selected == object)
					draw_highlight(rect2(pos, size), TO_RGB(vec3i(255, 164, 66)));

				if (highlighted == object)
					draw_highlight(rect2(pos, size), TO_RGB(vec3i(0, 255, 0)));
			}
		}*/

		
		if (get_focused())
			draw_highlight(area, Color(0, 1, 0));
		else
			draw_highlight(area, DEFAULT_THEME->get_selection_color());

		return_viewport->activate();
		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		if (viewport)
			viewport->resize(area);

		update();
		break;
	}
}

void WorldView::draw_grid()
{
	Color color = TO_RGB(50);

	if (handle_2d)
	{
		for (int c = -10; c <= 10; c++)
		{
			Color color = TO_RGB(50);

			if (c == 0)
				color = Color::Red;

			draw_line(vec2(-1000.0f, c * 100.0f), vec2(1000.0f, c * 100.0f), color);
		}
		for (int c = -10; c <= 10; c++)
		{
			Color color = TO_RGB(50);

			if (c == 0)
				color = Color::Green;

			draw_line(vec2(c * 100.0f, -1000.0f), vec2(c * 100.0f, 1000.0f), color);
		}
	}
	else
	{
		for (int c = -10; c <= 10; c++)
		{
			Color color = TO_RGB(250);

			if (c == 0)
				color = Color::Red;

			draw_line(vec2(-10.0f, c * 1.0f), vec2(10.0f, c * 1.0f), color);
		}
		for (int c = -10; c <= 10; c++)
		{
			Color color = TO_RGB(50);

			if (c == 0)
				color = Color::Green;

			draw_line(vec2(c * 1.0f, -10.0f), vec2(c * 1.0f, 10.0f), color);
		}
		color = Color::Blue;
		//draw_line(vec2(0.0f, -10.0f), vec2(c * 1.0f, 10.0f), color);
	}
}

void WorldView::handle_event(UIEvent *ui_event)
{
	vec2 pos = viewport->get_screen_coords(MOUSE->get_position());

	Object* n = viewport->get_canvas()->raycast(ui_event->pos);
	WorldObject* selected_worldobject = selected->cast_to_type<WorldObject*>();

	if (simulating)
		return;

	if (handle_2d)
	{
		if (ui_event->type == UIEvent::MOUSE_PRESS)
		{
			if (ui_event->press_type == UIEvent::UP)
			{
				item_dragging = false;
			}
			else
			{
				if (ui_event->button_type == Mouse::LEFT)
				{
					select(n->cast_to_type<WorldObject*>());

					item_dragging = true;
					prev_item_drag_pos = viewport->get_screen_coords(MOUSE->get_position());
				}
				else if (ui_event->button_type == Mouse::CENTER)
				{
					cam_dragging = ui_event->press_type == UIEvent::DOWN;
					prev_cam_drag_pos = ui_event->pos;
				}
			}
		}
		else if (ui_event->type == UIEvent::KEY_PRESS)
		{
			if (ui_event->press_type == Event::UP)
				return;

			if (ui_event->key == Key::KEY_RIGHT || ui_event->key == Key::KEY_LEFT || ui_event->key == Key::KEY_UP || ui_event->key == Key::KEY_DOWN)
			{
				Camera* cam = viewport->get_world()->get_active_camera();
				float velo = 20.0f;

				if (ui_event->key == Key::KEY_RIGHT)
					cam->move(vec3(velo, 0.0f, 0.0f));
				else if (ui_event->key == Key::KEY_LEFT)
					cam->move(vec3(-velo, 0.0, 0.0f));
				else if (ui_event->key == Key::KEY_UP)
					cam->move(vec3(0.0, velo, 0.0f));
				else if (ui_event->key == Key::KEY_DOWN)
					cam->move(vec3(0.0, -velo, 0.0f));

				cam->look_at(cam->get_pos() + vec3(0, 0, -1), vec3(0, 1, 0));
			}
		}
		else if (ui_event->type == UIEvent::MOUSE_HOVER && MOUSE->is_pressed(Mouse::LEFT) && item_dragging && selected)
		{
			vec2 current_drag_pos = viewport->get_screen_coords(MOUSE->get_position());

			vec2 drag_diff = current_drag_pos - prev_item_drag_pos;

			selected_worldobject->move(vec3(drag_diff, 0.0f));

			prev_item_drag_pos = current_drag_pos;
		}
		else if (ui_event->type == UIEvent::MOUSE_HOVER && MOUSE->is_pressed(Mouse::CENTER) && cam_dragging)
		{
			vec2 current_drag_pos = MOUSE->get_position();
			vec2 drag_diff = current_drag_pos - prev_cam_drag_pos;

			Camera* cam = viewport->get_world()->get_active_camera();
			cam->move(vec3(drag_diff * vec2(-1) * cam->get_zoom(), 0.0f));

			prev_cam_drag_pos = current_drag_pos;
			cam->look_at(cam->get_pos() + vec3(0, 0, -1), vec3(0, 1, 0));
		}
		else if (ui_event->type == UIEvent::MOUSE_HOVER)
		{
			highlight(n->cast_to_type<WorldObject*>());
		}
		else if (ui_event->type == UIEvent::MOUSE_SCROLL)
		{
			Camera* cam = viewport->get_world()->get_active_camera();

			if (ui_event->scroll_type == UIEvent::SCROLL_DOWN)
				cam->mult_zoom(1.5f);
			else
				cam->mult_zoom(0.75f);

			cam->look_at(cam->get_pos() + vec3(0, 0, -1), vec3(0, 1, 0));
		}
	}
	else
	{
		if (ui_event->type == UIEvent::MOUSE_PRESS && ui_event->button_type == Mouse::CENTER)
		{
			if (ui_event->button_type == Mouse::CENTER)
			{
				cam_dragging = ui_event->press_type == UIEvent::DOWN;
				prev_cam_drag_pos = ui_event->pos;
			}
		}
		else if (ui_event->type == UIEvent::MOUSE_PRESS || ui_event->type == UIEvent::MOUSE_HOVER)
		{
			DeferredRenderer* renderer = viewport->get_renderer()->cast_to_type<DeferredRenderer*>();
			Terrain* terrain = viewport->get_world()->get_child_by_type<Terrain*>();
			Raycaster r(viewport);
			vec3 p;
			vec2 sp = vec2(MOUSE->get_position() - viewport->get_area().get_pos());
			sp /= viewport->get_size();

			if (rect2(vec2(), vec2(1.0f)).is_in_box(sp))
			{
				sp += 1.0f;
				sp /= 2.0f;
				sp *= renderer->get_texture(DeferredRenderer::DEFERRED_POSITION)->get_size();

				vec3 m = renderer->get_material_at_pixel(sp);
				vec3 mouse_pos = raycast_fbo->read_pixel(sp, 0).get_rgb();
				float alias = 0.01f;

				if (ui_event->type == UIEvent::MOUSE_PRESS)
				{
					if (ui_event->press_type == UIEvent::DOWN)
					{
						drag_type = DRAG_INACTIVE;
						if (terrain)
							terrain->get_brush()->apply();

						if (m.z > 0.6f - alias && m.z < 0.6f + alias)
							drag_type = DRAG_X;
						else if (m.z > 0.7f - alias && m.z < 0.7f + alias)
							drag_type = DRAG_Y;
						else if (m.z > 0.8f - alias && m.z < 0.8f + alias)
							drag_type = DRAG_Z;


						if (selected && drag_type != DRAG_INACTIVE)
						{
							drag_start_grab = mouse_pos;
							drag_start_pos = selected_worldobject->get_pos();
							drag_start_size = selected_worldobject->get_size();
							drag_start_rotation = selected_worldobject->get_rotation();
						}
					}
					else
					{
						drag_type = DRAG_INACTIVE;
					}
				}
				else if (ui_event->type == UIEvent::MOUSE_HOVER && !MOUSE->is_pressed(MOUSE->LEFT))
				{
					highlight_type = DRAG_INACTIVE;

					if (m.z > 0.6f - alias && m.z < 0.6f + alias)
						highlight_type = DRAG_X;
					else if (m.z > 0.7f - alias && m.z < 0.7f + alias)
						highlight_type = DRAG_Y;
					else if (m.z > 0.8f - alias && m.z < 0.8f + alias)
						highlight_type = DRAG_Z;
				}
				else if (selected && MOUSE->is_pressed(MOUSE->LEFT) && drag_type != DRAG_INACTIVE)
				{
					if (transform_type == TRANSLATE)
					{
						if (drag_type == DRAG_X)
							selected_worldobject->set_pos(drag_start_pos + vec3(mouse_pos.x - drag_start_grab.x, 0, 0));
						else if (drag_type == DRAG_Y)
							selected_worldobject->set_pos(drag_start_pos + vec3(0, mouse_pos.y - drag_start_grab.y, 0));
						else if (drag_type == DRAG_Z)
							selected_worldobject->set_pos(drag_start_pos + vec3(0, 0, mouse_pos.z - drag_start_grab.z));
					}
					else if (transform_type == ROTATE)
					{

					}
					else if (transform_type == SCALE)
					{
						if (drag_type == DRAG_X)
							selected_worldobject->set_size(drag_start_size * vec3(mouse_pos.x / drag_start_grab.x, 1.0f, 1.0f));
						else if (drag_type == DRAG_Y)
							selected_worldobject->set_size(drag_start_size * vec3(1.0f, mouse_pos.y / drag_start_grab.y, 1.0f));
						else if (drag_type == DRAG_Z)
							selected_worldobject->set_size(drag_start_size * vec3(1.0f, 1.0f, mouse_pos.z / drag_start_grab.z));
					}
				}

				if (terrain)
				{
					p = renderer->get_position_at_pixel(sp);
					terrain->set_selection_pos(p.get_xy());
				}
			}
		}
		if (ui_event->type == UIEvent::KEY_PRESS)
		{
			if (ui_event->press_type == Event::UP)
				return;

			Terrain* terrain = viewport->get_world()->get_child_by_type<Terrain*>();

			if (ui_event->key == Key::KEY_P)
				TIME->game_paused = !TIME->game_paused;
			else if (ui_event->key == Key::KEY_K)
				TIME->game_time -= 10e6l;
			else if (ui_event->key == Key::KEY_L)
				TIME->game_time += 10e6;
			else if (ui_event->key == Key::KEY_T)
				RENDERER->use_wireframe();
			else if (terrain && ui_event->key == Key::KEY_0)
				terrain->get_brush()->apply();
			else if (terrain && ui_event->key == Key::KEY_1)
				terrain->increase(-1.0f);
			else if (terrain && ui_event->key == Key::KEY_2)
				terrain->compute_normals();

			/*if (terrain)
			{
				vec3 p = renderer->get_position_at_pixel(sp);
				terrain->set_selection_pos(p.get_xy());
			}*/
		}
	}

	if (viewport)
		viewport->handle_event(ui_event);

	update();
}

void WorldView::post_draw_world()
{
	Terrain* terrain = viewport->get_world()->get_child_by_type<Terrain*>();

	if (terrain)
		terrain->get_brush()->handle();

	if (!selected || !selected->derives_from_type<WorldObject*>())
		return;

	World* world = viewport->get_world();

	vec2 item_size = selected->cast_to_type<WorldObject*>()->get_size().get_xy() + vec2(2);
	vec2 item_pos = selected->cast_to_type<WorldObject*>()->get_pos().get_xy();
	vec2 handle_size = vec2(4.0f);
	vec2 offset = item_size + handle_size;

	DrawCommand command;
	command.type = DrawCommand::FRAME;
	command.area = rect2(item_pos, item_size);
	command.color = TO_RGB(vec3i(255, 164, 66));
	command.tex = CanvasData::get_singleton()->get_default_theme()->get_highlight();
	command.shader = CanvasData::get_singleton()->get_default_shader();

	render_frame(command);

	command.area = rect2(item_pos + offset, handle_size);
	render_box(command);
	command.area = rect2(item_pos - offset, handle_size);
	render_box(command);
	command.area = rect2(item_pos + offset * vec2(1, -1), handle_size);
	render_box(command);
	command.area = rect2(item_pos + offset * vec2(-1, 1), handle_size);
	render_box(command);

	// draw transformation handles

	vec3 pos = selected->cast_to_type<WorldObject*>()->get_pos();

	// draw invisible plane with position data

	raycast_fbo->bind();

	Transform t1, t2;
	vec3 size = vec3(100.0f, 100.0f, 0.0f);

	if (drag_type == DRAG_X || highlight_type == DRAG_X)
	{
		t1 = Transform(pos, size, vec3(0, 0, 0));
		t2 = Transform(pos, size, vec3(PI / 2, 0, 0));
	}
	else if (drag_type == DRAG_Y || highlight_type == DRAG_Y)
	{
		t1 = Transform(pos, size, vec3(0, 0, 0));
		t2 = Transform(pos, size, vec3(0, PI / 2, 0));
	}
	else if (drag_type == DRAG_Z || highlight_type == DRAG_Z)
	{
		t1 = Transform(pos, size, vec3(PI / 2, 0, 0));
		t2 = Transform(pos, size, vec3(0, PI / 2, 0));
	}

	raycast_shader->bind();
	raycast_shader->set_uniform("view", RENDERER->get_final_matrix());

	raycast_shader->set_uniform("model", t1.get_model());
	plane->bind();
	plane->draw();

	raycast_shader->set_uniform("model", t2.get_model());
	plane->draw();
	plane->unbind();

	DEFERRED_RENDERER->get_fbo(0)->bind();
	RENDERER->stop_depth_test();
	RENDERER->stop_culling();

	float highlight = 150.0f;

	if (transform_type == TRANSLATE)
	{
		//draw translation cones
		//X
		cone->set_pos(pos);
		cone->set_size(vec3(1.0f, .5f, .5f));
		cone->set_rotation(vec3(0, 0, 0));
		if (highlight_type == DRAG_X)
			cone->set_color(Color::Red);
		else
			cone->set_color(Color::FromRGB(vec3i(255, highlight, highlight)));
		cone->set_color_id(vec3(0, 0, 0.6f));
		cone->draw();

		//Y
		cone->set_rotation(vec3(0, 0, PI / 2));
		if (highlight_type == DRAG_Y)
			cone->set_color(Color::Green);
		else
			cone->set_color(Color::FromRGB(vec3i(highlight, 255, highlight)));
		cone->set_color_id(vec3(0, 0, 0.7f));
		cone->draw();

		//Z
		cone->set_rotation(vec3(0, PI / 2, 0));
		if (highlight_type == DRAG_Z)
			cone->set_color(Color::Blue);
		else
			cone->set_color(Color::FromRGB(vec3i(highlight, highlight, 255)));
		cone->set_color_id(vec3(0, 0, 0.8f));
		cone->draw();
	}
	else if (transform_type == ROTATE)
	{

	}
	else if (transform_type == SCALE)
	{
		//draw planes
		//X
		disk->set_pos(pos);
		disk->set_size(vec3(1, 0.5f, 0.5f));
		disk->set_rotation(vec3(0, 0, 0));
		if (highlight_type == DRAG_X)
			disk->set_color(Color::Red);
		else
			disk->set_color(Color::FromRGB(vec3i(255, highlight, highlight)));
		disk->set_color_id(vec3(0, 0, 0.6f));
		disk->draw();

		//Y
		disk->set_rotation(vec3(0, 0, PI / 2));
		if (highlight_type == DRAG_Y)
			disk->set_color(Color::Green);
		else
			disk->set_color(Color::FromRGB(vec3i(highlight, 255, highlight)));
		disk->set_color_id(vec3(0, 0, 0.7f));
		disk->draw();

		//Z
		disk->set_rotation(vec3(0, PI / 2, 0));
		if (highlight_type == DRAG_Z)
			disk->set_color(Color::Blue);
		else
			disk->set_color(Color::FromRGB(vec3i(highlight, highlight, 255)));
		disk->set_color_id(vec3(0, 0, 0.8f));
		disk->draw();
	}
	
	Camera* c = ACTIVE_VIEWPORT->get_world()->get_active_camera();
	RENDERER->use_depth_test(c->get_near(), c->get_far());
	RENDERER->use_culling();

	draw_grid();
}

void WorldView::post_draw_canvas()
{
}

void WorldView::set_postprocess(PostProcess* p_postprocess)
{
	postprocess = p_postprocess;
	p_postprocess->set_fbo(viewport->get_fbo());
}

PostProcess* WorldView::get_postprocess() const
{
	return postprocess;
}

void WorldView::set_scene(Scene* p_scene)
{
	viewport->set_scene(p_scene);
}

Scene* WorldView::get_scene() const
{
	return viewport->get_scene();
}

void WorldView::select(WorldObject* p_object)
{
	if (selected == p_object)
		return;

	selected = p_object;
	emit_signal("selected", selected);
}

WorldObject* WorldView::get_selected() const
{
	return selected->cast_to_type<WorldObject*>();
}

#undef CLASSNAME
#define CLASSNAME WorldView

void WorldView::highlight(WorldObject* p_object)
{
	if (highlighted == p_object)
		return;

	highlighted = p_object;
}

WorldObject* WorldView::get_highlight() const
{
	return highlighted->cast_to_type<WorldObject*>();
}

void WorldView::set_preview_type(int p_type)
{
	preview_type = p_type;
	preview_texture = viewport->get_renderer()->get_texture(p_type);
}

int WorldView::get_preview_type()
{
	return preview_type;
}

void WorldView::set_transform_type(int p_transform_type)
{
	transform_type = (TransformType)p_transform_type;
}

int WorldView::get_transform_type() const
{
	return transform_type;
}

void WorldView::set_simulating(bool p_simulating)
{
	simulating = p_simulating;
}

bool WorldView::get_simulating() const
{
	return simulating;
}

void WorldView::set_handle_2d(bool p_handle_2d)
{
	handle_2d = p_handle_2d;

	Camera* camera = viewport->get_world()->get_active_camera();

	if (handle_2d)
	{
		camera->set_ortho_projection(0.5, 5000.0);
		camera->set_rotation(vec3(-PI / 2.0, 0, 0));
	}
	else
		camera->set_projection(30.0f, 0.5f, 5000.0f);
}

bool WorldView::get_handle_2d() const
{
	return handle_2d;
}

WorldObject* WorldView::raycast(const vec2& p_pos) const
{
	Object* o = viewport->raycast(p_pos);

	return reinterpret_cast<WorldObject*>(o);
}

Viewport* WorldView::get_viewport() const
{
	return viewport;
}

#undef CLASSNAME
#define CLASSNAME WorldView

void WorldView::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(select);

	REG_PROPERTY(transform_type);

	REG_SIGNAL("selected");
	REG_SIGNAL("world_changed");
}