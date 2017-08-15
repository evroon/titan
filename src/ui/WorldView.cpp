#include "WorldView.h"

#include "graphics/Renderer.h"

WorldView::WorldView() : WorldView(NULL)
{

}

WorldView::WorldView(World* p_world)
{
	viewport = new Viewport();
	FBO2D* fbo = new FBO2D(vec2i(1024));
	
	viewport->set_canvas(new Canvas);
	viewport->set_world(p_world);
	viewport->set_mode(Viewport::FRAMEBUFFER);
	viewport->set_fbo(fbo);
	
	fbo->add_color_texture();
	fbo->add_depth_texture();
	fbo->init();

	selected = NULL;

	if (p_world)
	{
		get_world()->connect_signal("changed", this, "world_changed");

		p_world->get_active_camera()->set_name("Camera");
	}
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

	switch (p_notification)
	{
	case NOTIFICATION_READY:
		
		if (viewport)
		{
			viewport->init();
		}
		break;

	case NOTIFICATION_DRAW:

		if (viewport)
		{
			//viewport->draw();
		}

		RENDERER->use_scissor(area);
		//draw_texture(viewport->get_fbo()->color_textures[0]->cast_to_type<Texture2D*>(), get_area(), Color::White);
		RENDERER->stop_scissor();

		viewport->activate_transform();
		viewport->activate_world_transform();

		draw_grid();

		for (int c = 0; c < world->get_child_count(); c++)
		{
			WorldObject* object = world->get_child(c)->cast_to_type<WorldObject*>();

			if (object->is_of_type<Camera>())
				continue;
			
			vec3 size = object->get_transform().get_size();
			vec3 pos = object->get_transform().get_pos();

			if (selected == object)
				draw_highlight(rect2(pos.get_xy(), size.get_xy()), TO_RGB(vec3i(255, 164, 66)));

			if (highlighted == object)
				draw_highlight(rect2(pos.get_xy(), size.get_xy()), TO_RGB(vec3i(0, 255, 0)));
		}

		viewport->deactivate_world_transform();
		viewport->deactivate_transform();

		if (get_focused())
			draw_highlight(area, Color(0, 1, 0));
		else
			draw_highlight(area, DEFAULT_THEME->get_selection_color());

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		if (viewport)
			viewport->resize(area);
		
		break;
	}
}

void WorldView::draw_grid()
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

void WorldView::handle_event(UIEvent *ui_event)
{
	vec2 pos = viewport->get_screen_coords(MOUSE->get_position());

	Object* n = viewport->raycast(ui_event->pos);

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
	else if (ui_event->type == UIEvent::MOUSE_HOVER && MOUSE->is_pressed(Mouse::LEFT) && item_dragging && selected)
	{
		vec2 current_drag_pos = viewport->get_screen_coords(MOUSE->get_position());

		vec2 drag_diff = current_drag_pos - prev_item_drag_pos;

		selected->move(vec3(drag_diff, 0.0f));

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

	if (viewport)
		viewport->handle_event(ui_event);
}

void WorldView::set_postprocess(PostProcess* p_postprocess)
{
	postprocess = p_postprocess;
}

PostProcess* WorldView::get_postprocess() const
{
	return postprocess;
}

void WorldView::set_world(World* p_world)
{
	viewport->set_world(p_world);
}

World * WorldView::get_world()
{
	return viewport->get_world();
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
	return selected;
}

#undef CLASSNAME
#define CLASSNAME WorldView

void WorldView::highlight(WorldObject * p_object)
{
	if (highlighted == p_object)
		return;

	highlighted = p_object;
}

WorldObject * WorldView::get_highlight() const
{
	return highlighted;
}

void WorldView::bind_methods()
{
	REG_METHOD(select);

	REG_SIGNAL("selected");
	REG_SIGNAL("world_changed");
}