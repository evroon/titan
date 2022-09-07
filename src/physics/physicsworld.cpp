#include "physicsworld.h"

#include "core/time.h"

#include "rigidbody.h"
#include "core/definitions.h"

#if PLATFORM == WINDOWS
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#else
#include "bullet/btBulletDynamicsCommon.h"
#endif

PhysicsWorld3D::~PhysicsWorld3D()
{
	clean();
}

void PhysicsWorld3D::init()
{
	btBroadphaseInterface *broadphase = new btDbvtBroadphase();
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
	dynamics_world->setGravity(btVector3(0, 0, 0));
}

void PhysicsWorld3D::update()
{
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btCollisionShape* fallShape = new btSphereShape(1);
}

void PhysicsWorld3D::clean()
{
	delete dynamics_world;
	delete solver;
	delete dispatcher;
	delete collision_configuration;
	delete broadphase;
}

//=========================================================================
//PhysicsWorld2D
//=========================================================================

PhysicsWorld2D::PhysicsWorld2D()
{
	init();
}

void PhysicsWorld2D::DestroyWorld()
{
	boxes.clean();
	delete world;
}

void PhysicsWorld2D::destroy_box(RigidBody2D *body)
{
	//world->DestroyBody(body->shape->body);
	boxes.clean(body);
}

void PhysicsWorld2D::update()
{
	world->Step(1 / 60.0f, 8, 3);
}

void PhysicsWorld2D::init()
{
	scalefactor = 10.0f;
	scale = vec2(scalefactor);

	gravity = b2Vec2(0.0f, 0.0f);
	world = new b2World(gravity);

	world->SetContactListener(&collisiondata);
}

void PhysicsWorld2D::AddBody(RigidBody2D *body)
{
	boxes.push_back(body);
}

vec2 PhysicsWorld2D::get_scale() const
{
	return scale;
}
