#include "CollisionShape.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"

#include "world/WorldObject.h"
#include "math/TransformComponent.h"


//=========================================================================
//CollisionShape2D
//=========================================================================

#undef CLASSNAME
#define CLASSNAME CollisionShape2D

void CollisionShape2D::bind_methods()
{
}


//=========================================================================
//BoxBody
//=========================================================================

BoxShape2D::BoxShape2D(PhysicsWorld2D *world, bool dyn)
{
	shape.SetAsBox(1.0f, 1.0f);

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	body->CreateFixture(&fixdef);
}

//=========================================================================
//CircleBody
//=========================================================================

CircleShape2D::CircleShape2D(PhysicsWorld2D *world, bool dyn)
{
	shape.m_radius = 1.0f;

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	body->CreateFixture(&fixdef);
}