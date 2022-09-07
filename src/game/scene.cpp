#include "scene.h"

#include "graphics/view.h"

#include "graphics/renderer.h"
#include "game.h"

#include "input/eventhandler.h"

#include "scenemanager.h"
#include "core/windowmanager.h"

#include "ui/label.h"
#include "ui/canvas.h"

#include "core/serializer.h"

#include "world/world.h"
#include "world/model.h"
#include "world/terrain.h"
#include "world/sky.h"
#include "world/raycaster.h"

#include "input/keyboard.h"

Scene::Scene() : Scene("Scene")
{

}

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

}

void Scene::LoadContent()
{

}

void Scene::Start()
{
	Game::GetGame()->StartScene(this);
}

void Scene::update()
{

}

void Scene::Draw()
{
	VIEW->draw();
}

void Scene::handle_event(Event *e)
{

}

#undef CLASSNAME
#define CLASSNAME Scene

void Scene::bind_methods()
{
	REG_CSTR(0);

	REG_METHOD(Start);
}