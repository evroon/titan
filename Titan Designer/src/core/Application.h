#pragma once

#include "core/Object.h"
#include "core/platform/Platform.h"

class Window;
struct RenderTarget;

class Application : public Object
{
	OBJ_DEFINITION(Application, Object);

public:
	Platform *platform;

	Application(Platform *t)
	{
		platform = t;
		default_target = NULL;
	}

	virtual void init() { }
	virtual void update() { }
	virtual void draw() { }
	virtual void resize(const vec2i& p_size) { }

	void prepare_draw();
	void FinishDraw();

	void Loop();
	void InitSDL();
	void InitGL();
	void InitPhysics();
	void InitRenderer();
	void InitEngine();
	void Quit();
	void Free();

	Window *window;
	RenderTarget* default_target;
};