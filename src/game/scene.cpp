#include "scene.h"

#include "core/serializer.h"
#include "core/windowmanager.h"
#include "game.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "input/eventhandler.h"
#include "input/keyboard.h"
#include "scenemanager.h"
#include "ui/canvas.h"
#include "ui/label.h"
#include "world/model.h"
#include "world/raycaster.h"
#include "world/sky.h"
#include "world/terrain.h"
#include "world/world.h"

Scene::Scene() : Scene("Scene") {}

Scene::Scene(const String& p_name) {
    set_name(p_name);

    SCENEMANAGER->add_scene(this);
}

Scene::~Scene() { SCENEMANAGER->remove_scene(name); }

void Scene::Init() {}

void Scene::LoadContent() {}

void Scene::Start() { Game::GetGame()->StartScene(this); }

void Scene::update() {}

void Scene::Draw() { VIEW->draw(); }

void Scene::handle_event(Event* e) {}

#undef CLASSNAME
#define CLASSNAME Scene

void Scene::bind_methods() {
    REG_CSTR(0);

    REG_METHOD(Start);
}
