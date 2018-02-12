#include "EditorApp.h"

#include "core/Window.h"

#include "game/SceneManager.h"
#include "input/EventManager.h"
#include "input/Input.h"

#include "ui/Dock.h"
#include "ui/TextEditorTab.h"
#include "ui/ConsoleTab.h"
#include "ui/ExplorerTab.h"
#include "ui/TextField.h"
#include "ui/Slider.h"
#include "ui/TreeView.h"
#include "ui/PropertyView.h"
#include "ui/GamePreviewTab.h"
#include "ui/ToolTab.h"
#include "ui/ToolBar.h"
#include "ui/Dialog.h"
#include "ui/ContentTab.h"
#include "ui/PropertyTab.h"

#include "world/Model.h"
#include "world/Terrain.h"
#include "world/Sky.h"

#include "graphics/View.h"

#include "core/WindowManager.h"

#include "core\titanscript\TitanScript.h"

void EditorApp::init()
{
	Renderer* r = new ForwardRenderer;
	Viewport *v = new Viewport(r);
	Canvas* c = new Canvas;
	//Terrain* t = new Terrain;
	//t->set_name("terrain");
	v->set_name("viewport");

	VIEW->set_default_viewport(v);
	VIEW->set_active_viewport(v);

	v->set_canvas(c);

	active_project = new Project("Projects/second.xml");
	//active_project = new Project;
	//active_project->create();

	active_scene = active_project->get_main_scene();
	active_scene->add_child(new World);

	World* world = active_scene->get_child(0)->cast_to_type<World*>();
	world->set_name("World");
	world->attach_script(new TitanScript("Scripts/World.ts"));

	v->resize(WINDOWSIZE_F / 2.0f);
	v->set_mode(Viewport::DIRECT);

	//add controls
	main_dock = new Dock;
	content_dock = new Dock;
	explorer_dock = new Dock;
	property_dock = new Dock;
	//console_dock = new Dock;

	//add tabs
	game_preview_tab = new GamePreviewTab(world);
	WorldView* worldview = game_preview_tab->get_world_view();
	preview_viewport = worldview->get_viewport();
	preview_viewport->set_name("preview_viewport");

	content_tab = new ContentTab;
	explorer_tab = new ExplorerTab(preview_viewport);
	property_tab = new PropertyTab();
	console_tab = new ConsoleTab;
	//tool_tab = new ToolTab(t);

	//add bars
	bar = new ToolBar;

	c->add_control(main_dock);
	c->add_control(content_dock);
	c->add_control(explorer_dock);
	c->add_control(property_dock);
	//c->add_control(console_dock);
	c->add_control(bar);

	//main_dock
	main_dock->add_tab(game_preview_tab);
	main_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_END);
	main_dock->set_margins(0, 300, 500, 35);

	//content_dock
	content_dock->add_tab(content_tab);
	content_dock->add_tab(console_tab);
	content_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_BEGIN);
	content_dock->set_margins(4, 4, 500, 295);

	//explorer_dock
	explorer_dock->add_tab(explorer_tab);
	//explorer_dock->add_tab(tool_tab);
	explorer_dock->set_anchors(Control::ANCHOR_END, Control::ANCHOR_CENTER, Control::ANCHOR_END, Control::ANCHOR_END);
	explorer_dock->set_margins(495, 2, 4, 35);

	//property_dock
	property_dock->add_tab(property_tab);
	property_dock->set_anchors(Control::ANCHOR_END, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_CENTER);
	property_dock->set_margins(495, 4, 4, -2);

	//console_dock
	//console_dock->add_tab(console_tab);
	//console_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_BEGIN);
	//console_dock->set_margins(600, 4, 500, 195);

	//toolbar
	bar->add_item("File");
	bar->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_END, Control::ANCHOR_END);
	bar->set_margins(0, 30, 0, 0);

	//worldview->set_postprocess(new PostProcess(CONTENT->LoadShader("EngineCore/Shaders/FinalShader")));

	content_tab->connect("file_chosen", this, "open_file");

	explorer_tab->connect("selected", game_preview_tab->get_world_view(), "select");
	explorer_tab->connect("selected", property_tab, "set_property");

	preview_viewport->get_canvas()->set_name("Canvas");
	
	//Model* m = new Model("Models/a10/A10.dae");
	//m->set_pos(vec3(1000.0f, 1100.0f, 15.0f));
	//m->set_size(vec3(1.0f / 100.0f));
	//m->set_name("A10");

	//Water* water = new Water;
	//water->set_name("water");

	//Sky* sky = new Sky;
	//sky->set_name("sky");

	////Clouds* clouds = new Clouds;
	////clouds->set_name("clouds");

	//Vegetation* veg = new Vegetation(t);
	//veg->set_name("vegetation");

	/*world->add_worldobject(sky);
	world->add_worldobject(m);
	world->add_worldobject(t);
	world->add_worldobject(water);
	world->add_worldobject(veg);*/

	Camera* cam = world->get_active_camera();

	if (!cam)
	{
		cam = world->get_active_camera();

		if (!cam)
			cam = new Camera;

		world->set_active_camera(cam);
	}

	//cam->set_pos(vec3(600.0f, 2000.0f, 0.0f));
	//cam->set_rotation(vec3(0, 0, PI));

	//world->init();

	if (cam)
	{
		cam->set_pos(vec3(1000.0f, 1000.0f, 20.0f));
		cam->set_projection(30.0f, 0.5f, 5000.0f);
	}

	v->resize(WINDOWSIZE_F / 2.0f);

	explorer_tab->update_items();

	//active_project->save_as("Projects/second.xml");
}

void EditorApp::update()
{
	for (Event *e : INPUT->events)
		VIEW->handle_event(e);

	VIEW->update();
}

void EditorApp::draw()
{
	VIEW->draw();
}

void EditorApp::resize(const vec2i& p_size)
{
	VIEW->get_default_viewport()->resize(rect2(vec2(), vec2((float)p_size.x, (float)p_size.y) / 2.0f));
}

void EditorApp::open_file(const String& p_file)
{
	VariantType type = CONTENT->GetType(p_file);
	
	String name = type.get_type_name();

	if (name == "Shader")
	{
		main_dock->add_tab(new ShaderEditorTab(p_file));
	}
	else if (name == "TitanScript" || name == "TextFile")
	{
		main_dock->add_tab(new TextEditorTab(p_file));
	}
}

void EditorApp::set_viewport(Viewport* p_viewport)
{
	preview_viewport = p_viewport;
}

Viewport* EditorApp::get_viewport() const
{
	return preview_viewport;
}

#undef CLASSNAME
#define CLASSNAME EditorApp

void EditorApp::bind_methods()
{
	REG_METHOD(open_file);
}
