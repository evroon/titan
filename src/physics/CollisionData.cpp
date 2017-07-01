#include "CollisionData.h"

#include "core/titanscript/ScriptComponent.h"
#include "input/EventHandler.h"

void CollisionData::BeginContact(b2Contact* contact)
{
	RigidBody* A = static_cast<RigidBody*>(contact->GetFixtureA()->GetBody()->GetUserData());
	RigidBody* B = static_cast<RigidBody*>(contact->GetFixtureB()->GetBody()->GetUserData());

	CollisionEvent *e = new CollisionEvent;
	e->object = B;
	e->contact = CollisionEvent::BEGIN;

	WorldObject *woB = dynamic_cast<WorldObject*>(B->parent);
}

void CollisionData::EndContact(b2Contact* contact)
{
	RigidBody* A = static_cast<RigidBody*>(contact->GetFixtureA()->GetBody()->GetUserData());
	RigidBody* B = static_cast<RigidBody*>(contact->GetFixtureB()->GetBody()->GetUserData());

	CollisionEvent *e = new CollisionEvent;
	e->object = B;
	e->contact = CollisionEvent::END;

	WorldObject *woB = dynamic_cast<WorldObject*>(B->parent);
}