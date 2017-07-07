#include "Application.h"

#include "types/MethodMaster.h"
#include "WindowManager.h"
#include "input/Keyboard.h"
#include "Serializer.h"
#include "resources/XmlDocument.h"

#include "game/Scene.h"
#include "Time.h"
#include "graphics/VisualEffect.h"
#include "graphics/Renderer.h"
#include "Memory.h"
#include "input/Input.h"

#include "Window.h"
#include "Windows.h"
#include "core/platform/Android.h"

#include "core/platform/Platform.h"

#include "game/SceneManager.h"
#include "input/EventManager.h"

#include "NodeManager.h"

#include "physics/PhysicsGod.h"
#include "graphics/View.h"

void Application::InitSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//Titan::height = Titan::width = 1200;

	window = new Window(vec2i(2400, 1300));
	window->CreateGLContext();
}
void Application::InitGL()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
}
void Application::InitPhysics()
{
	PhysicsGod::BuildWorld();
}
void Application::InitRenderer()
{
	//Titan::updatetime = 1000.0 / 60.0;
	RENDERER->Init();
}
void Application::InitEngine()
{
	Serializer::init();
	CoreNames::init();

	Mouse::init();
	Keyboard::init();
	MessageHandler::init();
	ContentManager::Init();
	TypeManager::init();
	GarbageCollector::init();
	Input::init();
	EventManager::init();
	SceneManager::init();
	Math::init();

	INPUT->parent = this;

	T_INFO("Initializing SDL...");
	InitSDL();

	T_INFO("Initializing Platform...");
	platform->InitGL();
	
	T_INFO("Initializing Physics...");
	InitPhysics();
	
	T_INFO("Initializing Audio...");
	Audio::init();

	T_INFO("Initializing Content...");
	Font::Init();
	CONTENT->setup();	
	
	T_INFO("Initializing Renderer...");
	InitRenderer();

	T_LOG("Finished Initialization...");

	CanvasData::init();

	VIEW->init(vec2(to_float(window->size.x), to_float(window->size.y)) / 2.0f);
}

void Application::Free()
{
	INPUT->Clean();
}

void Application::PrepareDraw()
{
	RENDERER->Prepare();
}
void Application::FinishDraw()
{
	RENDERER->Finish();
}

void Application::Loop()
{
	Time::Init();
	InitEngine();
	init();

	SDL_Event event;
	bool running = true;

	FPSLimiter updatelim/*((int)Titan::updatetime)*/;
	Timer timer(1000);
	FPSLimiter::FPSInfo i;

	default_target = new RenderTarget;

	//Main Loop
	while (running)
	{
		TIME->Update();

		//Handle Events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				else if (
					event.key.keysym.sym == SDLK_F4 &&
					(event.key.keysym.mod == KMOD_LALT || event.key.keysym.mod == KMOD_RALT))
					running = false;
				else
					INPUT->HandleEvent(event);
			}
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				RENDERER->Resize(event.window.data1, event.window.data2);
				resize(vec2i(event.window.data1, event.window.data2));
			}
			else
				INPUT->HandleEvent(event);
		}

		if (timer.update())						//Print FPS each time after 1.0s
		{
			GC->clean();

			//restart timer
			timer.start();
		}

		//Handle Update Loop
		i = updatelim.update();

		if (i.needsupdate)						//Sync Update Loop to 60 FPS
		{
			TIME->OnUpdate();
			update();
			PhysicsGod::update();
			INPUT->Clean();
		}

		if (default_target->should_update())
		{
			PrepareDraw();
			draw();
			FinishDraw();

			window->SwapBuffer();					//Switch buffers			
		}

		GC->free();

	}

	//Quit
	Quit();
}

void Application::Quit()
{
	Free();
	NodeManager::Free();
	CONTENT->FreeAll();
	Primitives::Destroy();
	//NodeManager::Free();
	GC->free();
	StringUtils::Free();
	PhysicsGod::DestroyWorld();
	FBOMANAGER->Destroy();
	ERROR_HANDLER->clean();
	Audio::Free();
	VISUALEFFECT->free();
	RENDERER->Free();
	SCENEMANAGER->Free();
	EventManager::get_manager().clean();
	MethodMaster::clean();

	WINDOWMAN->clean();

	SDL_Quit();
}