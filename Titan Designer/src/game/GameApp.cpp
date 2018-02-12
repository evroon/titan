#include "GameApp.h"

#include "core/Project.h"
#include "graphics\View.h"
#include "graphics\Renderer.h"

void GameApp::init()
{
	Renderer* r = new DeferredRenderer;
	Viewport *v = new Viewport(r);
	VIEW->set_default_viewport(v);
	VIEW->set_active_viewport(v);

	Project* project = new Project("Projects/second.xml");
	start_scene(project->get_main_scene());
}

void GameApp::update()
{
	activescene->update();
}

void GameApp::draw()
{
	VIEW->draw();
}

void GameApp::start_scene(Scene *s)
{
	activescene = s;
	ACTIVE_VIEWPORT->set_world(activescene->get_child(0)->cast_to_type<World*>());

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