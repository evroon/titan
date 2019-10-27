#pragma once

#include "WorldObject.h"
#include "ui/Layer.h"
#include "Camera.h"
#include "core/Node.h"

class Viewport;
class PhysicsWorld2D;
class PhysicsWorld3D;
class DirectionalLight;

class World : public Node
{
	OBJ_DEFINITION(World, Node);

public:
	World();
	~World();

	void add_child(Node* p_child) override;
	void remove_child(Node* p_child) override;
	WorldObject* get_worldobject(const String &name);

	Viewport* get_viewport() const;

	Layer* GetLayer(const String &name);
	Layer* GetLayer(int index);

	void AddLayer(Layer *l);
	void RemoveLayer(Layer *l);
	void RepositionLayer(Layer *l);

	void handle_event(Event *e);
	void init();
	void update();
	void draw();
	void Free();
	
	void set_active_camera(Camera *p_camera);
	Camera* get_active_camera() const;

	DirectionalLight* get_active_light() const;

	PhysicsWorld2D* get_physics_2d() const;
	PhysicsWorld3D* get_physics_3d() const;

	Vector<Layer> layers;

	static void bind_methods();

private:
	Camera *active_camera;
	PhysicsWorld2D* physics_2d;
	PhysicsWorld3D* physics_3d;
};

