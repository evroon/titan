#include "WorldObject.h"

#include "graphics/RenderComponent.h"
#include "math/TransformComponent.h"
#include "core/titanscript/ScriptComponent.h"
#include "physics/PhysicsWorld.h"
#include "physics/RigidBody.h"
#include "World.h"

WorldObject::WorldObject()
{
	set_transformcomponent(new TransformComponent);
	set_rendercomponent(new RenderComponent);
}

WorldObject::~WorldObject()
{
	if (world)
		world->remove_child(this);

	if (layer)
		layer->UnBind(this);

	components.clean();
}

void WorldObject::handle_event(Event *e)
{
	if (method_exists(CORE_NAMES->handle_event))
	{
		run(CORE_NAMES->handle_event, Arguments(e));
		return;
	}
}

void WorldObject::notificate(int notification)
{
	if (has_script())
	{
		StringName sname;

		switch (notification)
		{
		case NOTIFICATION_READY:
			sname = CORE_NAMES->ready;
			break;

		case NOTIFICATION_DRAW:
			sname = CORE_NAMES->draw;
			break;

		case NOTIFICATION_UPDATE:
			sname = CORE_NAMES->update;
			break;
		}

		if (method_exists(sname))
		{
			run(sname, Arguments());
		}
	}

	switch (notification)
	{
	case NOTIFICATION_READY:
		ready();
		break;

	case NOTIFICATION_DRAW:
		draw();
		break;

	case NOTIFICATION_DRAW_SHADOW:
		shadow_draw();
		break;

	case NOTIFICATION_UPDATE:
		update();
		break;
	}
}

void WorldObject::update()
{

}

void WorldObject::set_transformcomponent(TransformComponent *c)
{
	transformcomponent = c;

	if (transformcomponent)
		components.push_back(c);
}

void WorldObject::set_rendercomponent(RenderComponent *c)
{
	rendercomponent = c;

	if (rendercomponent)
	{
		components.push_back(c);
	}
}

//void WorldObject::set_physicscomponent(PhysicsComponent2D *c)
//{
//	physicscomponent = c;
//
//	if (physicscomponent)
//	{
//		components.push_back(c);
//		c->bind_parent(this);
//	}
//}

TransformComponent* WorldObject::get_transformcomponent() const
{
	return transformcomponent;
}

RenderComponent* WorldObject::get_rendercomponent() const
{
	return rendercomponent;
}

//PhysicsComponent2D* WorldObject::get_physicscomponent() const
//{
//	return physicscomponent;
//}

Transform WorldObject::get_transform() const
{
	return transformcomponent->transform;
}
void WorldObject::set_transform(const Transform &p_transform) const
{
	transformcomponent->transform = p_transform;
}

vec3 WorldObject::get_pos() const
{
	return get_transform().get_pos();
}

void WorldObject::set_pos(const vec3 &p_pos)
{
	Transform &transform = transformcomponent->transform;

	transform.set_pos(p_pos);
	transform.update();

	//if (physicscomponent)
	//	physicscomponent->set_transform(transform);
}

vec3 WorldObject::get_size() const
{
	return transformcomponent->transform.get_size();
}

void WorldObject::set_size(const vec3 &p_size)
{
	Transform &transform = transformcomponent->transform;

	transform.set_size(p_size);
	transform.update();


	//if (physicscomponent)
	//	physicscomponent->set_transform(transform);

}

vec3 WorldObject::get_rotation() const
{
	return transformcomponent->transform.get_rotation();
}

void WorldObject::set_rotation(const vec3& p_rotation)
{
	Transform &transform = transformcomponent->transform;

	transform.set_rotation(p_rotation);
	transform.update();

	//if (physicscomponent)
	//	physicscomponent->set_transform(transform);
}

vec3 WorldObject::get_axis() const
{
	vec3 p = get_pos();
	vec3 t = (transformcomponent->get_transform().get_model() * vec4(0, 1, 0, 1)).get_xyz();
	return t - vec3(0, 0, 0);
}

void WorldObject::move(const vec3 & p_delta)
{
	set_pos(get_pos() + p_delta);
}

void WorldObject::rotate(const quat& p_quat)
{
	set_rotation((get_transform().get_quat() * p_quat).get_euler());
}

Color& WorldObject::get_color() const
{
	return rendercomponent->get_color();
}
void WorldObject::set_color(const Color &p_color)
{
	rendercomponent->set_color(p_color);
}

bool& WorldObject::get_visible() const
{
	return rendercomponent->isvisible;
}
void WorldObject::set_visible(const bool b)
{
	rendercomponent->isvisible = b;
}

Layer* WorldObject::get_layer() const
{
	return layer;
}
void WorldObject::set_layer(Layer *l)
{
	if (layer)
		layer->UnBind(this);

	layer = l;

	if (layer)
		layer->Bind(this);
}

World* WorldObject::get_world() const
{
	return world;
}

void WorldObject::register_in_world(World *p_world)
{
	world = p_world;
}

void WorldObject::free()
{
	GC->queue_free(this);
}

template<typename T> T WorldObject::GetComponent()
{
	for (int c = 0; c < components.size(); c++)
		if (typeid(T) == typeid(components[c]))
			return components[c];
}


void WorldObject::set_component(Component *c)
{
	if (c->is_of_type<TransformComponent>())
		set_transformcomponent(reinterpret_cast<TransformComponent*>(c));
	else if (c->is_of_type<RenderComponent>())
		set_rendercomponent(reinterpret_cast<RenderComponent*>(c));
	//else if (c->is_of_type<PhysicsComponent2D>())
	//	set_physicscomponent(reinterpret_cast<PhysicsComponent2D*>(c));

	c->bind_parent(this);
	components.push_back(c);
}

bool WorldObject::CheckOverlap(const vec2 &p) const
{
	vec3 pos = get_pos();
	vec3 size = get_size();

	return	p.x > pos.x - size.x &&
		p.x < pos.x + size.x &&
		p.y > pos.y - size.y &&
		p.y < pos.y + size.y;
}

#undef CLASSNAME
#define CLASSNAME WorldObject

void WorldObject::bind_methods()
{
	REG_PROPERTY(pos);
	REG_PROPERTY(size);
	REG_PROPERTY(rotation);

	REG_PROPERTY(color);
	//REG_PROPERTY(layer);
	REG_PROPERTY(visible);
	REG_PROPERTY(transform);

	REG_PROPERTY(transformcomponent);
	REG_PROPERTY(rendercomponent);

	REG_METHOD(move);
	REG_METHOD(free);

	REG_METHOD(get_world);
}