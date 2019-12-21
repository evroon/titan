#include "Scene.h"

#include "graphics/View.h"

#include "graphics/Renderer.h"
#include "Game.h"

#include "input/EventHandler.h"

#include "SceneManager.h"
#include "core/WindowManager.h"

#include "ui/Label.h"
#include "ui/Canvas.h"

#include "core/Serializer.h"

#include "world/World.h"
#include "world/Model.h"
#include "world/Terrain.h"
#include "world/Sky.h"
#include "world/Raycaster.h"

#include "input/Keyboard.h"

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