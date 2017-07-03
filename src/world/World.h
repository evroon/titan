#pragma once

#include "WorldObject.h"
#include "ui/Layer.h"
#include "Camera.h"
#include "core/Node.h"

class Viewport;

class World2D : public Node
{
	OBJ_DEFINITION(World2D, Node);

public:
	World2D();
	~World2D();

	void add_worldobject(WorldObject *world_object);
	void remove_world_object(WorldObject *world_object);
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

	Vector<Layer> layers;

	static void bind_methods();

private:
	Camera *active_camera;
	Camera *light_camera;
};

