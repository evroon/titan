#include "PhysicsGod.h"

#include "Time.h"

b2World * PhysicsGod::world;
b2AABB PhysicsGod::universesize;
b2Vec2 PhysicsGod::grav;
Vector<RigidBody> PhysicsGod::boxes;

vec2 PhysicsGod::scaling;
float PhysicsGod::scalefactor;

CollisionData PhysicsGod::collisiondata;

void PhysicsGod::DestroyWorld()
{
	boxes.clean();
	delete world;
}

void PhysicsGod::DestroyBox(RigidBody *body)
{
	world->DestroyBody(body->body);
	boxes.clean(body);
}

void PhysicsGod::update()
{
	world->Step(1 / 60.0f, 8, 3);
}

void PhysicsGod::BuildWorld()
{
	scalefactor = 30.0;
	scaling = vec2(scalefactor);

	grav = b2Vec2(0.0f, -9.81f);
	world = new b2World(grav);

	world->SetContactListener(&collisiondata);
}

void PhysicsGod::AddBody(RigidBody *body)
{
	boxes.push_back(body);
}