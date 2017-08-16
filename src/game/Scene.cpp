#include "Scene.h"

#include "graphics/View.h"

#include "graphics/Renderer.h"
#include "Game.h"

#include "input/EventHandler.h"

#include "SceneManager.h"
#include "core/WindowManager.h"

#include "ui/Label.h"

#include "core/Serializer.h"

#include "world/Model.h"
#include "world/Terrain.h"
#include "world/Sky.h"

#include "input/Keyboard.h"

#include "world/Raycaster.h"

Scene::Scene(const String &p_name)
{
	set_name(p_name);

	SCENEMANAGER->add_scene(this);
}

Scene::~Scene()
{
	SCENEMANAGER->remove_scene(name);
}

void Scene::Init()
{	
	LoadContent();
	
	Viewport* v = ACTIVE_VIEWPORT;
	v->set_postprocess(new PostProcess);
	v->set_fbo(RENDERER->get_render_buffer());

	//mouse_position = MOUSE->get_position();

	if (v)
	{
		World* w = new World;

		v->set_world(w);

		Model* m = new Model("assets/Models/672a8302d72f34f79980861feb2bdcd5/untitled.dae");
		m->set_pos(vec3(50, 50, 25));
		m->set_size(vec3(10.0f));
		m->set_name("plane");

		Terrain* t = new Terrain();
		t->set_name("terrain");

		Water* water = new Water;
		water->set_name("water");

		Sky* sky = new Sky;

		Clouds* c = new Clouds;
		c->set_name("clouds");
		
		Vegetation* veg = new Vegetation(t);

		w->add_worldobject(sky);
		w->add_worldobject(m);
		w->add_worldobject(t);
		w->add_worldobject(water);
		w->add_worldobject(veg);
		w->add_worldobject(c);

		w->get_active_camera()->set_rotation(vec3(0, 0, PI));

		w->init();
	}
}

void Scene::LoadContent()
{
	//run("Init");
}

void Scene::Start()
{
	Game::GetGame()->StartScene(this);
}

void Scene::update()
{
	Camera* c = VIEW->get_active_viewport()->get_world()->get_active_camera();

	c->handle_input();
}

void Scene::Draw()
{	
	VIEW->draw();
}

void Scene::handle_event(Event *e)
{
	InputEvent* input = reinterpret_cast<InputEvent*>(e);

	if (!input)
		return;

	switch (input->type)
	{
	case InputEvent::KEYPRESS:



		break;
	}
}

#undef CLASSNAME
#define CLASSNAME Scene

void Scene::bind_methods()
{
	REG_METHOD(Start);
}