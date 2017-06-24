#pragma once

#include "core/Application.h"

#include "resources/File.h"

class World2D;
class Dock;
class TextEditorTab;
class GamePreviewTab;
class ContentTab;
class ExplorerTab;
class PropertyTab;
class ConsoleTab;
class ToolBar;

struct EditorCommand
{
	StringName name;
};

class EditorApp : public Application
{
	OBJ_DEFINITION(EditorApp, Application);

public:
	EditorApp(Platform *t) : Application(t) { }

	void init() override;
	void update() override;
	void draw() override;
	void resize(const vec2i& p_size) override;
	
	void open_file(const String & p_file);

	void set_world2d(World2D* p_world2d);
	World2D* get_world2d() const;

	static void bind_methods();

private:
	//controls
	Dock* main_dock;
	Dock* content_dock;
	Dock* explorer_dock;
	Dock* property_dock;
	Dock* console_dock;

	//add tabs
	TextEditorTab* text_edit_tab;
	GamePreviewTab* game_preview_tab;
	ContentTab* content_tab;
	ExplorerTab* explorer_tab;
	PropertyTab* property_tab;
	ConsoleTab* console_tab;

	//add bars
	ToolBar* bar;

	World2D* world2d;
};

