#pragma once

#include "core/Object.h"
#include "core/Definitions.h"
#include "Box2D/Box2D.h"

#if PLATFORM == WINDOWS
#include "btBulletDynamicsCommon.h"
#else
#include "bullet/btBulletDynamicsCommon.h"
#endif

class RigidBody2D;
class RigidBody3D;
class WorldObject;
class b2Body;
class PhysicsWorld2D;

class CollisionShape3D : public Object
{
	OBJ_DEFINITION(CollisionShape3D, Object);

public:
	CollisionShape3D();
	virtual ~CollisionShape3D();

	friend class RigidBody3D;

	static void bind_methods();

protected:
	btCollisionShape* shape;
};

class PlaneShape3D : CollisionShape3D
{
	OBJ_DEFINITION(PlaneShape3D, Object);

public:
	PlaneShape3D();

	static void bind_methods();

private:

};

class SphereShape3D : CollisionShape3D
{
	OBJ_DEFINITION(SphereShape3D, Object);

public:
	SphereShape3D();

	static void bind_methods();

private:

};

class BoxShape3D : CollisionShape3D
{
	OBJ_DEFINITION(SphereShape3D, Object);

public:
	BoxShape3D();

	static void bind_methods();

private:

};

class CollisionShape2D : public Object
{
	OBJ_DEFINITION(CollisionShape2D, Object);

public:
	CollisionShape2D();
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
	BoxShape2D(PhysicsWorld2D *world, WorldObject* p_object, bool dyn = true);
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
	CircleShape2D(PhysicsWorld2D *world, WorldObject* p_object, bool dyn = true);
	virtual ~CircleShape2D() { }

protected:
	b2CircleShape shape;
};