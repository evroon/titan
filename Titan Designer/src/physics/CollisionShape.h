#pragma once

#include "core/Object.h"

#include "Box2D/Box2D.h"

class RigidBody2D;
class WorldObject;
class b2Body;
class PhysicsWorld2D;

class CollisionShape3D : public Object
{
	OBJ_DEFINITION(CollisionShape3D, Object);

public:

	static void bind_methods();

private:

};

class PlaneShape3D : CollisionShape3D
{
	OBJ_DEFINITION(PlaneShape3D, Object);

public:

	static void bind_methods();

private:

};

class SphereShape3D : CollisionShape3D
{
	OBJ_DEFINITION(SphereShape3D, Object);

public:

	static void bind_methods();

private:

};

class BoxShape3D : CollisionShape3D
{
	OBJ_DEFINITION(SphereShape3D, Object);

public:

	static void bind_methods();

private:

};

class CollisionShape2D : public Object
{
	OBJ_DEFINITION(CollisionShape2D, Object);

public:
	static void bind_methods();

	b2Body *body;

protected:
	b2BodyDef bodydef;
	b2FixtureDef fixdef;

private:
};

//2D Box
class BoxShape2D : public CollisionShape2D
{
	OBJ_DEFINITION(BoxShape2D, CollisionShape2D)

public:
	BoxShape2D() = default;
	BoxShape2D(PhysicsWorld2D *world, bool dyn = true);
	virtual ~BoxShape2D() { }

protected:
	b2PolygonShape shape;
};

//2D Circle
class CircleShape2D : public CollisionShape2D
{
	OBJ_DEFINITION(CircleShape2D, CollisionShape2D)

public:
	CircleShape2D() = default;
	CircleShape2D(PhysicsWorld2D *world, bool dyn = true);
	virtual ~CircleShape2D() { }

protected:
	b2CircleShape shape;
};