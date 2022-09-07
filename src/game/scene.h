#pragma once

#include "core/node.h"
#include "input/event.h"

class Scene : public Node
{
	OBJ_DEFINITION(Scene, Node);

public:
	Scene();
	Scene(const String &name);
	virtual ~Scene();

	void Init();
	void LoadContent();
	void Start();
	void update();
    void Draw();
    void handle_event(Event *e);

	static void bind_methods();

private:
};