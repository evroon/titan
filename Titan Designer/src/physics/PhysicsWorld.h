#pragma once

/*
2D and 3D Physics Implementation
Started: 24-12-2015
Units: SI-units
Updates in a discrete manner at 60FPS
Libraries: Box2D, Bullet
*/

#include "Box2D\Box2D.h"
#include "btBulletDynamicsCommon.h"

#include "CollisionData.h"

class PhysicsWorld3D
{
public:
	PhysicsWorld3D() = default;
	~PhysicsWorld3D();

	friend class RigidBody3D;

	void init();
	void update();
	void clean();

	vec3 get_scale() const;

private:
	vec2 scale;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collision_configuration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamics_world;
};

class PhysicsWorld2D
{
public:
	void DestroyWorld();
	void destroy_box(RigidBody2D *body);
	void update();
	void init();
	void AddBody(RigidBody2D *body);

	vec2 get_scale() const;

private:
	b2World *world;
	b2AABB universesize;
	b2Vec2 gravity;

	Vector<RigidBody2D> boxes;

	vec2 scale;
	float scalefactor;
	CollisionData collisiondata;
};