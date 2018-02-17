#pragma once

/*
2D Physics Implementation
Erik Vroon
Started: 24-12-2015
Units: SI-units
Updates in a discrete manner at 60FPS
*/

#include <iostream>
#include <vector>

#include "RigidBody.h"
#include "Box2D\Box2D.h"

#include "CollisionData.h"


class PhysicsGod
{
public:
	static void DestroyWorld();
	static void DestroyBox(RigidBody *body);
	static void update();
	static void BuildWorld();
	static void AddBody(RigidBody *body);

	static b2World *world;
	static b2AABB universesize;
	static b2Vec2 grav;

	static Vector<RigidBody> boxes;

	static vec2 scaling;
	static float scalefactor;
	static CollisionData collisiondata;
};