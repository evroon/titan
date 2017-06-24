#include "PhysicsComponent.h"

#include "Box2D\Box2D.h"

#include "world/WorldObject.h"

#include "PhysicsGod.h"

#include "RigidBody.h"

PhysicsComponent::PhysicsComponent()
{

}

PhysicsComponent::~PhysicsComponent()
{
	PhysicsGod::DestroyBox(body);
}

void PhysicsComponent::update()
{
	if (body)
		body->update();
}

vec2 PhysicsComponent::get_velocity() const
{
	b2Vec2 velo = body->body->GetLinearVelocity();
	return vec2(velo.x, velo.y) * PhysicsGod::scaling;
}

void PhysicsComponent::set_velocity(const vec2 &p_velocity)
{
	vec2 velo = p_velocity / PhysicsGod::scaling;

	body->body->SetLinearVelocity(b2Vec2(velo.x, velo.y));
}

bool PhysicsComponent::get_fixed_rotation() const
{
	return body->body->IsFixedRotation();
}

void PhysicsComponent::set_fixed_rotation(bool p_value)
{
	body->body->SetFixedRotation(p_value);
}

void PhysicsComponent::set_transform(const Transform &p_transform)
{
	vec2 pos = p_transform.get_pos().get_xy() / PhysicsGod::scaling;
	
	if (body)
	{
		body->body->SetTransform(b2Vec2(pos.x, pos.y), p_transform.get_rotation().z);
		body->body->SetAwake(true);
	}
}

void PhysicsComponent::set_as_box(bool p_dynamic)
{
	if (parent)
		body = new BoxBody(PhysicsGod::world, parent, p_dynamic);
	else
		T_ERROR("Component has no parent");
}

void PhysicsComponent::set_as_circle(bool p_dynamic)
{
	body = new CircleBody(PhysicsGod::world, parent, p_dynamic);
}

void PhysicsComponent::set_as_sensor(bool p_value)
{
	body->set_as_sensor(p_value);
}

WorldObject* PhysicsComponent::get_colliding_objects() const
{
	b2ContactEdge *bce = body->body->GetContactList();

	if (!bce)
		return NULL;

	b2Contact *c = bce->contact;
	WorldObject *o = reinterpret_cast<WorldObject*>(c->GetFixtureB()->GetBody()->GetUserData());

	return o;
}

#undef CLASSNAME
#define CLASSNAME PhysicsComponent

void PhysicsComponent::bind_methods()
{
	REG_CSTR(0);

	REG_METHOD(set_as_box);
	REG_METHOD(set_as_circle);

	REG_METHOD(set_as_sensor);

	REG_METHOD(get_colliding_objects);

	REG_PROPERTY(velocity);
	REG_PROPERTY(fixed_rotation);
}