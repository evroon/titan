#pragma once

#include "math/Transform.h"
#include "core/Component.h"
#include "core/Vector.h"
#include "math/Color.h"
#include "ui/Layer.h"
#include "input/Event.h"
#include "core/Node.h"

class TransformComponent;
class ScriptComponent;
class RenderComponent;
class PhysicsComponent;
class World;

class WorldObject : public Node
{
	OBJ_DEFINITION(WorldObject, Node);

public:
	WorldObject();
	virtual ~WorldObject();

	enum NotificationType
	{
		NOTIFICATION_READY,
		NOTIFICATION_DRAW,
		NOTIFICATION_DRAW_SHADOW,
		NOTIFICATION_UPDATE,
		NOTIFICATION_RESIZED
	};

	virtual void ready() { }
	virtual void handle_event(Event *e);
	virtual void update();
	virtual void SimpleDraw() { }
	virtual void draw() { }
	virtual void shadow_draw() { }

	//Get or set the most important properties
	Transform get_transform() const;
	void set_transform(const Transform &p_transform) const;

	vec3 get_pos() const;
	void set_pos(const vec3 &p_pos);

	vec3 get_size() const;
	void set_size(const vec3 &p_size);

	vec3 get_rotation() const;
	void set_rotation(const vec3& p_rotation);
	vec3 get_axis() const;

	void move(const vec3 &p_delta);
	void rotate(const quat& p_quat);

	Color& get_color() const;
	void set_color(const Color &p_color);

	bool& get_visible() const;
	void set_visible(const bool b);

	Layer* get_layer() const;
	void set_layer(Layer *l);

	World* get_world() const;

	void register_in_world(World *p_world);

	void free();

	void notificate(int notification);

	//Get component
	template<typename T> T GetComponent();

	//Add components
	void set_component(Component *c);
	void set_transformcomponent(TransformComponent *c);
	void set_rendercomponent(RenderComponent *c);
	void set_physicscomponent(PhysicsComponent *c);

	TransformComponent* get_transformcomponent() const;
	RenderComponent* get_rendercomponent() const;
	PhysicsComponent* get_physicscomponent() const;

	//Components
	Vector<Component> components;
	TransformComponent *transformcomponent;
	RenderComponent *rendercomponent;
	PhysicsComponent *physicscomponent;

	//Check Overlap of bounding box with a vector
	virtual bool CheckOverlap(const vec2 &p) const;

	static void bind_methods();
	
	World *world = NULL;
	Layer *layer = NULL;
};