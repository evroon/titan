#include "RigidBody.h"

#include "PhysicsGod.h"

#include "world/WorldObject.h"
#include "math/TransformComponent.h"

//RigidBody
RigidBody::RigidBody(b2World *world, WorldObject *obj, bool dyn = true)
{
	Transform trans = obj->get_transform();

	dynamic = dyn;
	size = trans.get_size().get_xy() / vec2(2.0f) / PhysicsGod::scalefactor;

	bodydef.position.Set(trans.get_pos().x / PhysicsGod::scalefactor, trans.get_pos().y / PhysicsGod::scalefactor);

	if (dyn)
		bodydef.type = b2_dynamicBody;

	body = world->CreateBody(&bodydef);
	body->SetUserData(obj);

	parent = obj;

	PhysicsGod::AddBody(this);
}

void RigidBody::update()
{
	if (!parent)
		return;

	Transform &t = parent->transformcomponent->transform;

	b2Vec2 pos = body->GetPosition();

	t.set_pos2d(vec2(pos.x, pos.y) * PhysicsGod::scaling);
	t.set_size2d(size * PhysicsGod::scaling);
	t.set_rotation(body->GetAngle());
}

void RigidBody::set_as_sensor(bool p_value)
{
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
		f->SetSensor(p_value);
}

//Box
BoxBody::BoxBody(b2World *world, WorldObject *obj, bool dyn) : RigidBody(world, obj, dyn)
{
	shape.SetAsBox(size.x, size.y);

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	body->CreateFixture(&fixdef);

	update();
}

//Circle
CircleBody::CircleBody(b2World *world, WorldObject *obj, bool dyn) : RigidBody(world, obj, dyn)
{
	shape.m_radius = size.x;

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	body->CreateFixture(&fixdef);

	update();
}