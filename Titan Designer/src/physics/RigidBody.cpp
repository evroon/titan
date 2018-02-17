#include "RigidBody.h"

#include "PhysicsWorld.h"
#include "CollisionShape.h"

#include "world/World.h"

#include "world/WorldObject.h"
#include "math/TransformComponent.h"

//=========================================================================
//RigidBody3D
//=========================================================================



#undef CLASSNAME
#define CLASSNAME RigidBody3D

void RigidBody3D::bind_methods()
{
}

//=========================================================================
//RigidBody2D
//=========================================================================

RigidBody2D::RigidBody2D(b2World *world, bool dyn = true)
{
	physics_2d = get_world_object()->get_world()->get_physics_2d();

	Transform trans = get_world_object()->get_transform();

	dynamic = dyn;
	size = trans.get_size().get_xy() / vec2(2.0f) / physics_2d->get_scale();

	//bodydef.position.Set(trans.get_pos().x / physics_2d->get_scale().x, trans.get_pos().y / physics_2d->get_scale().y);

	//if (dyn)
	///	bodydef.type = b2_dynamicBody;

	//shape->body = world->CreateBody(&bodydef);
	//shape->body->SetUserData(parent);
	
	physics_2d->AddBody(this);
}

void RigidBody2D::update()
{
	if (!parent)
		return;

	Transform &t = get_world_object()->transformcomponent->transform;

	b2Vec2 pos = shape->body->GetPosition();

	t.set_pos2d(vec2(pos.x, pos.y) * physics_2d->get_scale());
	t.set_size2d(size * physics_2d->get_scale());
	t.set_rotation(shape->body->GetAngle());
}

void RigidBody2D::set_as_sensor(bool p_value)
{
	for (b2Fixture* f = shape->body->GetFixtureList(); f; f = f->GetNext())
		f->SetSensor(p_value);
}

vec2 RigidBody2D::get_velocity() const
{
	b2Vec2 velo = shape->body->GetLinearVelocity();
	return vec2(velo.x, velo.y) * physics_2d->get_scale();
}

void RigidBody2D::set_velocity(const vec2 &p_velocity)
{
	vec2 velo = p_velocity / physics_2d->get_scale();

	shape->body->SetLinearVelocity(b2Vec2(velo.x, velo.y));
}

bool RigidBody2D::get_fixed_rotation() const
{
	return shape->body->IsFixedRotation();
}

void RigidBody2D::set_fixed_rotation(bool p_value)
{
	shape->body->SetFixedRotation(p_value);
}

void RigidBody2D::set_transform(const Transform &p_transform)
{
	vec2 pos = p_transform.get_pos().get_xy() / physics_2d->get_scale();

	if (shape->body)
	{
		shape->body->SetTransform(b2Vec2(pos.x, pos.y), p_transform.get_rotation().z);
		shape->body->SetAwake(true);
	}
}

void RigidBody2D::set_as_box(bool p_dynamic)
{
	if (parent)
		shape = new BoxShape2D(physics_2d, p_dynamic);
	else
		T_ERROR("Component has no parent");
}

void RigidBody2D::set_as_circle(bool p_dynamic)
{
	shape = new CircleShape2D(physics_2d, p_dynamic);
}

WorldObject* RigidBody2D::get_colliding_objects() const
{
	b2ContactEdge *bce = shape->body->GetContactList();

	if (!bce)
		return NULL;

	b2Contact *c = bce->contact;
	WorldObject *o = reinterpret_cast<WorldObject*>(c->GetFixtureB()->GetBody()->GetUserData());

	return o;
}

#undef CLASSNAME
#define CLASSNAME RigidBody2D

void RigidBody2D::bind_methods()
{
	REG_CSTR(0);

	REG_METHOD(set_as_box);
	REG_METHOD(set_as_circle);

	REG_METHOD(set_as_sensor);

	REG_METHOD(get_colliding_objects);

	REG_PROPERTY(velocity);
	REG_PROPERTY(fixed_rotation);
}

WorldObject* RigidBody2D::get_world_object()
{
	return get_parent()->cast_to_type<WorldObject*>();
}
