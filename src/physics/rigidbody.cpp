#include "rigidbody.h"

#include "collisionshape.h"
#include "math/transformcomponent.h"
#include "physicsworld.h"
#include "world/world.h"
#include "world/worldobject.h"

//=========================================================================
// RigidBody3D
//=========================================================================

RigidBody3D::RigidBody3D() {
    btDefaultMotionState* groundMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, shape->shape,
                                                               btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    physics_3d->dynamics_world->addRigidBody(groundRigidBody);

    btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    shape->shape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape->shape,
                                                             fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    physics_3d->dynamics_world->addRigidBody(fallRigidBody);
}

#undef CLASSNAME
#define CLASSNAME RigidBody3D

void RigidBody3D::bind_methods() {}

//=========================================================================
// RigidBody2D
//=========================================================================

RigidBody2D::RigidBody2D(bool dyn) {
    shape = NULL;
    physics_2d = NULL;

    dynamic = dyn;

    auto ready = [this]() { get_parent()->connect("parent_changed", this, "ready"); };
    connect("parent_changed", Connection::create_from_lambda(new V_Method_0(ready)));
}

void RigidBody2D::ready() {
    if (!get_world_object()) {
        T_ERROR("World object was null");
        return;
    } else if (!get_world_object()->get_world()) {
        T_ERROR("World was null");
        return;
    }
    physics_2d = get_world_object()->get_world()->get_physics_2d();

    Transform trans = get_world_object()->get_transform();

    size = trans.get_size().get_xy() / vec2(2.0f) / physics_2d->get_scale();

    physics_2d->AddBody(this);
}

void RigidBody2D::update() {
    if (!shape || !shape->body) return;

    b2Vec2 pos = shape->body->GetPosition();
    vec2 new_pos = vec2(pos.x, pos.y) * physics_2d->get_scale();

    get_world_object()->set_pos(vec3(new_pos, get_world_object()->get_pos().z));
    get_world_object()->set_rotation(vec3(0.0f, 0.0f, -shape->body->GetAngle()));
}

void RigidBody2D::set_as_sensor(bool p_value) {
    for (b2Fixture* f = shape->body->GetFixtureList(); f; f = f->GetNext()) f->SetSensor(p_value);
}

vec2 RigidBody2D::get_velocity() const {
    if (!physics_2d || !shape || !shape->body) return vec2();

    b2Vec2 velo = shape->body->GetLinearVelocity();
    return vec2(velo.x, velo.y) * physics_2d->get_scale();
}

void RigidBody2D::set_velocity(const vec2& p_velocity) {
    if (!physics_2d || physics_2d == nullptr || !shape || !shape->body) return;

    vec2 velo = p_velocity / physics_2d->get_scale();

    shape->body->SetLinearVelocity(b2Vec2(velo.x, velo.y));
}

bool RigidBody2D::get_fixed_rotation() const {
    if (!shape || !shape->body) return false;

    return shape->body->IsFixedRotation();
}

void RigidBody2D::set_fixed_rotation(bool p_value) {
    if (shape && shape->body) shape->body->SetFixedRotation(p_value);
}

void RigidBody2D::set_transform(const Transform& p_transform) {
    vec2 pos = p_transform.get_pos().get_xy() / physics_2d->get_scale();

    if (shape && shape->body) {
        shape->body->SetTransform(b2Vec2(pos.x, pos.y), p_transform.get_rotation().z);
        shape->body->SetAwake(true);
    }
}

void RigidBody2D::set_as_box(bool p_dynamic) {
    if (physics_2d)
        shape = new BoxShape2D(physics_2d, get_world_object(), p_dynamic);
    else
        T_ERROR("Component has no parent, could not set RigidBody2D as box.");
}

void RigidBody2D::set_as_circle(bool p_dynamic) {
    shape = new CircleShape2D(physics_2d, get_world_object(), p_dynamic);
}

void RigidBody2D::apply_force(const vec2& p_force) {
    shape->body->ApplyLinearImpulse(b2Vec2(p_force.x, p_force.y), shape->body->GetWorldCenter(),
                                    true);
}

WorldObject* RigidBody2D::get_colliding_objects() const {
    b2ContactEdge* bce = shape->body->GetContactList();

    if (!bce) return nullptr;

    b2Contact* c = bce->contact;
    WorldObject* o =
        reinterpret_cast<WorldObject*>(c->GetFixtureB()->GetBody()->GetUserData().pointer);

    return o;
}

#undef CLASSNAME
#define CLASSNAME RigidBody2D

void RigidBody2D::bind_methods() {
    REG_CSTR(0);
    REG_CSTR_OVRLD_1(bool);

    REG_METHOD(ready);

    REG_METHOD(set_as_box);
    REG_METHOD(set_as_circle);

    REG_METHOD(set_as_sensor);

    REG_METHOD(get_colliding_objects);

    REG_METHOD(apply_force);

    REG_PROPERTY(velocity);
    REG_PROPERTY(fixed_rotation);
}

WorldObject* RigidBody2D::get_world_object() {
    Node* parent = get_parent();
    if (parent == NULL) return nullptr;

    return parent->cast_to_type<WorldObject*>();
}
