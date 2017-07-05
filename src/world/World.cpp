#include "World.h"

#include "graphics/View.h"
#include "graphics/Renderer.h"

#include "world/Terrain.h"

#include "core/CoreNames.h"

World2D::World2D()
{
	AddLayer(new Layer(0, "DefaultLayer"));

	active_camera = new Camera;
	add_worldobject(active_camera);

	light_camera = new Camera;
	add_worldobject(light_camera);

	light_camera->set_pos(vec3(50.0f, 50.0f, 200.0f));
	light_camera->set_projection(30.0f, 0.5f, 1000.0f);
	light_camera->set_rotation(vec3(-PI / 2.0f, 0, PI));
}


World2D::~World2D()
{
}

void World2D::add_worldobject(WorldObject *world_object)
{
	add_child(world_object);
	world_object->register_in_world(this);
	world_object->set_layer(GetLayer(0));

	world_object->notificate(WorldObject::NOTIFICATION_READY);

	emit_signal("changed");
}

void World2D::remove_world_object(WorldObject *world_object)
{
	children.clear(world_object);
	emit_signal("changed");
}

WorldObject* World2D::get_worldobject(const String &name)
{
	for (Node* obj : children)
	{
		WorldObject* w = obj->cast_to_type<WorldObject*>();

		if (w->name == name)
			return w;
	}

	return NULL;
}

Viewport* World2D::get_viewport() const
{
	return ACTIVE_VIEWPORT;
}

Layer* World2D::GetLayer(const String &name)
{
	for (int c = 0; c < layers.size(); c++)
		if (layers[c]->name == name)
			return layers[c];

	return NULL;
}
Layer* World2D::GetLayer(int depth)
{
	for (int c = 0; c < layers.size(); c++)
		if (layers[c]->depth == depth)
			return layers[c];

	return NULL;
}
void World2D::AddLayer(Layer *l)
{
	int destination = -1;

	for (int c = 0; c < layers.size(); c++)
	{
		if (layers[c]->depth > l->depth)
		{
			destination = c;
			break;
		}
	}

	if (destination == -1)
		layers.push_back(l);
	else
		layers.insert(destination, l);
}
void World2D::RemoveLayer(Layer *l)
{
	layers.clear(l);
}

void World2D::RepositionLayer(Layer *l)
{
	RemoveLayer(l);
	AddLayer(l);
}

void World2D::handle_event(Event *e)
{
	for (Node* p : children)
		p->cast_to_type<WorldObject*>()->handle_event(e);
}

void World2D::init()
{
	if (has_script())
		run(CORE_NAMES->ready, Arguments());

	if (active_camera)
	{
		active_camera->set_pos(vec3(50.0f, 50.0f, 20.0f));
		active_camera->set_projection(30.0f, 0.5f, 1000.0f);
	}
}

void World2D::update()
{
	for (Layer *l : layers)
		for (WorldObject *wo : l->objects)
			wo->notificate(WorldObject::NOTIFICATION_UPDATE);
}

void World2D::draw()
{
	//draw shadowmap
	FBOMANAGER->BindFBO(RENDERER->get_shadow_buffer());

	RENDERER->switch_to_camera(light_camera);

	mat4 light_matrix = RENDERER->get_final_matrix();

	for (Layer *l : layers)
		for (WorldObject *wo : l->objects)
			wo->notificate(WorldObject::NOTIFICATION_DRAW_SHADOW);	

	RENDERER->deactivate_camera();

	//draw refection
	Camera* c = get_active_camera();

	vec3 p = c->get_pos();
	c->set_pos(vec3(p.x, p.y, 0 - p.z));

	vec3 r = c->get_rotation();
	c->set_rotation(vec3(-r.x, r.y, r.z));

	c->update();

	FBOMANAGER->BindFBO(RENDERER->get_reflection_buffer());

	RENDERER->switch_to_camera(c);

	for (Layer *l : layers)
	{
		for (WorldObject *wo : l->objects)
		{
			if (!dynamic_cast<Water*>(wo))
				wo->notificate(WorldObject::NOTIFICATION_DRAW);
		}
	}

	RENDERER->deactivate_camera();

	c->set_rotation(r);
	c->set_pos(p);

	c->update();

	//draw scene
	FBOMANAGER->BindFBO(RENDERER->get_render_buffer());
	RENDERER->set_light_matrix(light_matrix);

	for (Layer *l : layers)
		for (WorldObject *wo : l->objects)
			wo->notificate(WorldObject::NOTIFICATION_DRAW);
}

void World2D::Free()
{
	children.clean();
}

void World2D::set_active_camera(Camera *p_camera)
{
	active_camera = p_camera;
}

Camera* World2D::get_active_camera() const
{
	return active_camera;
}

#undef CLASSNAME
#define CLASSNAME World2D

void World2D::bind_methods()
{
	REG_METHOD(add_worldobject);

	REG_METHOD(get_viewport);

	REG_SIGNAL("changed");
}
