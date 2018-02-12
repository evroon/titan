#pragma once

#include "Box2D\Box2D.h"
#include "world/WorldObject.h"
#include "RigidBody.h"

class CollisionData : public b2ContactListener
{
public:	
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};