#pragma once

#include "scene.h"
#include "core/time.h"
#include "graphics/postprocess.h"
#include "graphics/renderer.h"
#include "core/memory.h"
#include "input/input.h"

#include "core/window.h"
#include "core/platform/windows.h"
#include "core/platform/android.h"

#include "graphics/viewport.h"

#include "core/application.h"

#include "scenemanager.h"

class Game
{
public:
	Game();

	Scene* LoadScene(const String &name);
	void StartScene(Scene *s);
	void handle_event(Event *e);

	void Start();
	void update();
	void Draw();

	Scene *activescene;

	//Get activegame
	static Game* GetGame();

private:
	static Game *activegame;
};