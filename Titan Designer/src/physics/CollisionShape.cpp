#include "CollisionShape.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"

#include "world/WorldObject.h"
#include "math/TransformComponent.h"


//=========================================================================
//CollisionShape3D
//=========================================================================

CollisionShape3D::CollisionShape3D()
{
}

CollisionShape3D::~CollisionShape3D()
{

}

#undef CLASSNAME
#define CLASSNAME CollisionShape3D

void CollisionShape3D::bind_methods()
{
}

//=========================================================================
//BoxShape3D
//=========================================================================

BoxShape3D::BoxShape3D()
{
}

#undef CLASSNAME
#define CLASSNAME BoxShape3D

void BoxShape3D::bind_methods()
{
}

//=========================================================================
//SphereShape3D
//=========================================================================

SphereShape3D::SphereShape3D()
{
	shape = new btSphereShape(1);
}

#undef CLASSNAME
#define CLASSNAME SphereShape3D

void SphereShape3D::bind_methods()
{
}

//=========================================================================
//PlaneShape3D
//=========================================================================

PlaneShape3D::PlaneShape3D()
{
	//shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
}

#undef CLASSNAME
#define CLASSNAME PlaneShape3D

void PlaneShape3D::bind_methods()
{
}


//=========================================================================
//CollisionShape2D
//=========================================================================

#undef CLASSNAME
#define CLASSNAME CollisionShape2D

CollisionShape2D::CollisionShape2D()
{

}

void CollisionShape2D::bind_methods()
{
}


//=========================================================================
//BoxBody
//=========================================================================

BoxShape2D::BoxShape2D(PhysicsWorld2D *p_world, WorldObject* p_object, bool dyn)
{
	const Transform& transform = p_object->get_transform();
	shape.SetAsBox(transform.get_size().x, transform.get_size().y);

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	fixdef.restitution = 1.0f;
	fixdef.friction = 0.0f;

	bodydef.type = dyn ? b2_dynamicBody : b2_staticBody;
	bodydef.position.Set(transform.get_pos().x, transform.get_pos().y);

	body = p_world->world->CreateBody(&bodydef);
	body->CreateFixture(&fixdef);
	body->SetUserData(p_object);
}

//=========================================================================
//CircleBody
//=========================================================================

CircleShape2D::CircleShape2D(PhysicsWorld2D *p_world, WorldObject* p_object, bool dyn)
{
	const Transform& transform = p_object->get_transform();
	shape.m_radius = transform.get_size().x;

	fixdef.shape = &shape;
	fixdef.density = 1.0;
	fixdef.restitution = 1.0f;
	fixdef.friction = 0.0f;

	bodydef.type = dyn ? b2_dynamicBody : b2_staticBody;
	bodydef.position.Set(transform.get_pos().x, transform.get_pos().y);

	body = p_world->world->CreateBody(&bodydef);
	body->CreateFixture(&fixdef);
	body->SetUserData(p_object);
}