#pragma once

#include "core/Node.h"
#include "graphics/Viewport.h"
#include "input/Event.h"


class Scene : public Node
{
	OBJ_DEFINITION(Scene, Node);
	  
public:
	Scene(const String &name);
	virtual ~Scene();

	void Init();
	void LoadContent();
	void Start();
	void update();
    void Draw();
    void handle_event(Event *e);
	
	Viewport* default_viewport;

	static void bind_methods();

private:
	vec2 mouse_position;
};