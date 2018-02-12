#pragma once

#include "Box2D\Box2D.h"

#include "core/Object.h"

class WorldObject;

class RigidBody : public Object
{
	OBJ_DEFINITION(RigidBody, Object)

public:
	RigidBody() = default;
	RigidBody(b2World *world, WorldObject *obj, bool dyn);
	virtual ~RigidBody() { }

	void update();

	void BindParent(WorldObject *p_parent) { parent = p_parent; }

	void set_as_sensor(bool p_value);

	b2Body *body;
	WorldObject *parent;

protected:
	b2BodyDef bodydef;
	b2FixtureDef fixdef;

	vec2 size;
	bool dynamic;

};


//2D Box
class BoxBody : public RigidBody
{
	OBJ_DEFINITION(BoxBody, RigidBody)

public:
	BoxBody() = default;
	BoxBody(b2World *world, WorldObject *obj, bool dyn = true);
	virtual ~BoxBody() { }

protected:
	b2PolygonShape shape;
};

//2D Circle
class CircleBody : public RigidBody
{
	OBJ_DEFINITION(CircleBody, RigidBody)

public:
	CircleBody() = default;
	CircleBody(b2World *world, WorldObject *obj, bool dyn = true);
	virtual ~CircleBody() { }

protected:
	b2CircleShape shape;
};