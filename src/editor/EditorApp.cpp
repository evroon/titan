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
#include "ui/ToolBar.h"
#include "ui/Dialog.h"
#include "ui/ContentTab.h"
#include "ui/PropertyTab.h"

#include "world/Model.h"
#include "world/Terrain.h"
#include "world/Sky.h"

#include "graphics/View.h"

void EditorApp::init()
{
	Canvas* c = ACTIVE_CANVAS;
	Viewport* v = ACTIVE_VIEWPORT;

	v->set_mode(Viewport::DIRECT);

	world = new World;
	world->set_name("World");

	Sprite* sprite = new Sprite("Textures/grass.png");
	sprite->set_name("test_sprite");
	sprite->set_size(vec3(100, 100, 1));

	//add controls
	main_dock = new Dock;
	content_dock = new Dock;
	explorer_dock = new Dock;
	property_dock = new Dock;
	console_dock = new Dock;

	//add tabs
	//text_edit_tab = new TextEditorTab("");
	game_preview_tab = new GamePreviewTab(world);
	content_tab = new ContentTab;
	explorer_tab = new ExplorerTab(world);
	property_tab = new PropertyTab();
	console_tab = new ConsoleTab;

	//add bars
	bar = new ToolBar;

	c->add_control(main_dock);
	c->add_control(content_dock);
	c->add_control(explorer_dock);
	c->add_control(property_dock);
	c->add_control(console_dock);
	c->add_control(bar);

	//main_dock
	//main_dock->add_tab(text_edit_tab);
	main_dock->add_tab(game_preview_tab);
	main_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_END);
	main_dock->set_margins(300, 200, 500, 35);

	//content_dock
	content_dock->add_tab(content_tab);
	content_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END);
	content_dock->set_margins(4, 4, 295, 35);

	//explorer_dock
	explorer_dock->add_tab(explorer_tab);
	explorer_dock->set_anchors(Control::ANCHOR_END, Control::ANCHOR_CENTER, Control::ANCHOR_END, Control::ANCHOR_END);
	explorer_dock->set_margins(495, 2, 4, 35);

	//property_dock
	property_dock->add_tab(property_tab);
	property_dock->set_anchors(Control::ANCHOR_END, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_CENTER);
	property_dock->set_margins(495, 4, 4, -2);

	//console_dock
	console_dock->add_tab(console_tab);
	console_dock->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_BEGIN);
	console_dock->set_margins(300, 4, 500, 195);

	//toolbar
	bar->add_item("File");
	bar->set_anchors(Control::ANCHOR_BEGIN, Control::ANCHOR_END, Control::ANCHOR_END, Control::ANCHOR_END);
	bar->set_margins(0, 30, 0, 0);

	game_preview_tab->get_world_view()->connect("world_changed", explorer_tab, "update_items");
	game_preview_tab->get_world_view()->connect("selected", explorer_tab, "select");
	game_preview_tab->get_world_view()->connect("selected", property_tab, "set_property");

	explorer_tab->connect("selected", game_preview_tab->get_world_view(), "select");
	explorer_tab->connect("selected", property_tab, "set_property");

	content_tab->connect("file_chosen", this, "open_file");

	World* w = world;

	v->set_world(w);

	Model* m = new Model("assets/Models/672a8302d72f34f79980861feb2bdcd5/untitled.dae");
	m->set_pos(vec3(50, 50, 25));
	m->set_size(vec3(10.0f));
	m->set_name("plane");

	Terrain* t = new Terrain();
	t->set_name("terrain");

	Water* water = new Water;
	water->set_name("water");

	Sky* sky = new Sky;

	Clouds* clouds = new Clouds;
	clouds->set_name("clouds");

	Vegetation* veg = new Vegetation(t);

	w->add_worldobject(sky);
	w->add_worldobject(m);
	w->add_worldobject(t);
	w->add_worldobject(water);
	w->add_worldobject(veg);
	w->add_worldobject(clouds);

	w->get_active_camera()->set_rotation(vec3(0, 0, PI));

	w->init();
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

void EditorApp::resize(const vec2i & p_size)
{
	ACTIVE_VIEWPORT->resize(rect2(vec2(), vec2((float)p_size.x, (float)p_size.y) / 2.0f));
}

void EditorApp::open_file(const String& p_file)
{
	VariantType type = CONTENT->GetType(p_file);
	
	String name = type.get_type_name();

	if (name == "Shader" || name == "TitanScript" || name == "TextFile")
	{
		main_dock->add_tab(new TextEditorTab(p_file));
	}
}

void EditorApp::set_world2d(World * p_world2d)
{
	world = p_world2d;
}

World * EditorApp::get_world2d() const
{
	return world;
}

#undef CLASSNAME
#define CLASSNAME EditorApp

void EditorApp::bind_methods()
{
	REG_METHOD(open_file);
}
