#pragma once

#include "core/application.h"
#include "core/project.h"
#include "resources/file.h"

class Viewport;
class World;
class WorldObject;
class Dock;
class TextEditorTab;
class GamePreviewTab;
class ContentTab;
class ExplorerTab;
class PropertyTab;
class ConsoleTab;
class ToolTab;
class ToolBar;
class FileDialog;

struct EditorCommand {
    StringName name;
};

class EditorApp : public Application {
    OBJ_DEFINITION(EditorApp, Application);

   public:
    EditorApp(Platform* t) : Application(t) {}

    void init() override;
    void update() override;
    void draw() override;
    void resize(const vec2i& p_size) override;

    void open_file(const String& p_file);

    void set_viewport(Viewport* p_viewport);
    Viewport* get_viewport() const;

    void save();
    void save_as();
    void load();
    void project_options();
    void exit();

    static void bind_methods();

   private:
    Project* active_project;
    Scene* active_scene;

    // controls
    Dock* main_dock;
    Dock* content_dock;
    Dock* explorer_dock;
    Dock* property_dock;

    // add tabs
    GamePreviewTab* game_preview_tab;
    ContentTab* content_tab;
    ExplorerTab* explorer_tab;
    PropertyTab* property_tab;
    ConsoleTab* console_tab;
    ToolTab* tool_tab;

    // add bars
    ToolBar* bar;

    WorldObject* selected;

    Viewport* preview_viewport;
};
