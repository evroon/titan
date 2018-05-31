#include "GameApp.h"

#include "core/Project.h"
#include "graphics/View.h"
#include "graphics/Renderer.h"
#include "input/Input.h"

void GameApp::init()
{
	Renderer* r = new ForwardRenderer;
	Viewport *v = new Viewport(r);
	VIEW->set_default_viewport(v);
	VIEW->set_active_viewport(v);

	Project* project = new Project("Projects/jumper.xml");
	start_scene(project->get_main_scene());
}

void GameApp::update()
{
	activescene->update();

	for (Event *e : INPUT->events)
		VIEW->handle_event(e);

	VIEW->update();
}

void GameApp::draw()
{
	VIEW->draw();
}

void GameApp::start_scene(Scene *s)
{
	activescene = s;
	ACTIVE_VIEWPORT->set_scene(activescene);

	Camera* cam = ACTIVE_VIEWPORT->get_world()->get_active_camera();
	if (!cam)
	{
		cam = new Camera;
		ACTIVE_VIEWPORT->get_world()->add_child(cam);
		ACTIVE_VIEWPORT->get_world()->set_active_camera(cam);
	}
	
	cam->set_pos(vec3(0.0f, -20.0f, 0.0f));
	cam->set_projection(30.0f, 0.5f, 5000.0f);	
}

void GameApp::handle_event(Event *e)
{
	activescene->handle_event(e);
}