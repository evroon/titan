#pragma once

#include "box2d/box2d.h"
#include "core/node.h"

class WorldObject;
class PhysicsWorld2D;
class PhysicsWorld3D;
class CollisionShape2D;
class CollisionShape3D;

class RigidBody3D : public Node {
    OBJ_DEFINITION(RigidBody3D, Node);

   public:
    RigidBody3D();

    friend class CollisionShape3D;

    static void bind_methods();

   private:
    PhysicsWorld3D* physics_3d;

    CollisionShape3D* shape;
};

class RigidBody2D : public Node {
    OBJ_DEFINITION(RigidBody2D, Node)

   public:
    RigidBody2D(bool dynamic = false);
    virtual ~RigidBody2D() {}

    void ready();
    void update();

    vec2 get_velocity() const;
    void set_velocity(const vec2& p_velocity);

    bool get_fixed_rotation() const;
    void set_fixed_rotation(bool p_value);

    void set_transform(const Transform& p_transform);

    void set_as_box(bool p_dynamic);
    void set_as_circle(bool p_dynamic);

    void apply_force(const vec2& p_force);

    void set_as_sensor(bool p_value);

    WorldObject* get_colliding_objects() const;

    static void bind_methods();

   protected:
    WorldObject* get_world_object();

    CollisionShape2D* shape;

    PhysicsWorld2D* physics_2d;

    vec2 size;
    bool dynamic;
};
