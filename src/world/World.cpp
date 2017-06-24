#include "World.h"

#include "View.h"

#include "CoreNames.h"

World2D::World2D()
{
	AddLayer(new Layer(0, "DefaultLayer"));

	active_camera = new Camera;
	add_worldobject(active_camera);
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
		active_camera->set_pos(vec3(50.0f, 50.0f, -2.0f));
		active_camera->set_projection(30.0f, .1f, 1000.0f);
		active_camera->look_at(active_camera->get_pos() + vec3(0, 1, 0), vec3(0, 0, 1));
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
