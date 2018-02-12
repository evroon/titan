#include "GameApp.h"

#include "core/Project.h"
#include "core/Window.h"
#include "graphics/View.h"

void GameApp::init()
{
	Project* project = new Project("Projects/first.xml");
	Scene* s = project->get_main_scene();

	if (s)
	{
		World* w = s->get_child("World")->cast_to_type<World*>();
		if (!w)
			T_ERROR("Could not find world");

		ACTIVE_VIEWPORT->set_world(w);
	}
	else
	{
		T_ERROR("Could not load scene");
	}
}

void GameApp::update()
{
	activescene->update();
}

void GameApp::start_scene(Scene *s)
{
	activescene = s;
}

void GameApp::handle_event(Event *e)
{
	activescene->handle_event(e);
}