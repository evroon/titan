#pragma once

#include "core/Component.h"

class RigidBody;

class PhysicsComponent : public Component
{
	OBJ_DEFINITION(PhysicsComponent, Component)

public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	void update() override;

	vec2 get_velocity() const;
	void set_velocity(const vec2 & p_velocity);

	bool get_fixed_rotation() const;
	void set_fixed_rotation(bool p_value);

	void set_transform(const Transform &p_transform);

	void set_as_box(bool p_dynamic);
	void set_as_circle(bool p_dynamic);

	void set_as_sensor(bool p_value);

	WorldObject* get_colliding_objects() const;
	
	static void bind_methods();

private:
	RigidBody *body;

};