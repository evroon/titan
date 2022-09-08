#pragma once

#include "core/application.h"
#include "core/memory.h"
#include "core/platform/android.h"
#include "core/platform/windows.h"
#include "core/time.h"
#include "core/window.h"
#include "graphics/postprocess.h"
#include "graphics/renderer.h"
#include "graphics/viewport.h"
#include "input/input.h"
#include "scene.h"
#include "scenemanager.h"

class Game {
   public:
    Game();

    Scene* LoadScene(const String& name);
    void StartScene(Scene* s);
    void handle_event(Event* e);

    void Start();
    void update();
    void Draw();

    Scene* activescene;

    // Get activegame
    static Game* GetGame();

   private:
    static Game* activegame;
};
