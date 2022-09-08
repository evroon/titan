#pragma once

#include "scene.h"

#define SCENEMANAGER SceneManager::get_singleton()

class SceneManager : public Object {
    OBJ_DEFINITION(SceneManager, Object);

   public:
    SceneManager();
    ~SceneManager();

    void add_scene(Scene* ps_scene);
    Scene* get_scene(const String& p_name);
    void remove_scene(const String& p_name);
    bool DoesSceneExist(const String& p_name);

    void Free();

    Vector<Scene> scenes;

    static void init();

    static SceneManager* get_singleton();

    static void bind_methods();

   private:
    static SceneManager* singleton;
};
