#include "scenemanager.h"

SceneManager* SceneManager::singleton;

SceneManager::SceneManager() { scenes = Vector<Scene>(); }

SceneManager::~SceneManager() {}

void SceneManager::add_scene(Scene* p_name) { scenes.push_back(p_name); }

Scene* SceneManager::get_scene(const String& p_name) {
    for (int c = 0; c < scenes.size(); c++)
        if (scenes[c]->get_name() == p_name) return scenes[c];

    return nullptr;
}

void SceneManager::remove_scene(const String& p_name) {
    for (int c = 0; c < scenes.size(); c++) {
        if (scenes[c]->get_name() == p_name) {
            scenes.clear(c);
            return;
        }
    }

    T_ERROR("Scene does not exist: " + p_name);
}

bool SceneManager::DoesSceneExist(const String& p_name) { return get_scene(p_name) != NULL; }

void SceneManager::Free() { scenes.clean(); }

void SceneManager::init() { singleton = new SceneManager; }

SceneManager* SceneManager::get_singleton() { return singleton; }

#undef CLASSNAME
#define CLASSNAME SceneManager

void SceneManager::bind_methods() {}
