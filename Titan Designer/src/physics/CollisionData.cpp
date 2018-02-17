#include "CollisionData.h"

#include "core/titanscript/ScriptComponent.h"
#include "input/EventHandler.h"

void CollisionData::BeginContact(b2Contact* contact)
{
	RigidBody2D* A = static_cast<RigidBody2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
	RigidBody2D* B = static_cast<RigidBody2D*>(contact->GetFixtureB()->GetBody()->GetUserData());

	CollisionEvent *e = new CollisionEvent;
	e->object = B;
	e->contact = CollisionEvent::BEGIN;

	WorldObject *woB = dynamic_cast<WorldObject*>(B->get_parent());
}

void CollisionData::EndContact(b2Contact* contact)
{
	RigidBody2D* A = static_cast<RigidBody2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
	RigidBody2D* B = static_cast<RigidBody2D*>(contact->GetFixtureB()->GetBody()->GetUserData());

	CollisionEvent *e = new CollisionEvent;
	e->object = B;
	e->contact = CollisionEvent::END;

	WorldObject *woB = dynamic_cast<WorldObject*>(B->get_parent());
}