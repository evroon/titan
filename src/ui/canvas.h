#pragma once

#include "control.h"

class Viewport;
class Dialog;
class ContextMenu;
class ContextTip;
class Shader;

class CanvasLayer {
   public:
    CanvasLayer(int p_level);
    ~CanvasLayer();

    Vector<Control> controls;
    int level;
};

class Canvas : public Node {
    OBJ_DEFINITION(Canvas, Node);

   public:
    Canvas();
    ~Canvas();

    void schedule_update(Control* p_control);

    void add_layer();
    void remove_layer(int p_index);

    void add_control(Control* p_control, int p_level = 0);
    void remove_control(Control* p_control);

    void add_control_to_top(Control* p_control);

    void set_dialog(Dialog* p_dialog);
    void remove_dialog();

    void set_context_tip(const String& p_description, const vec2& pos);
    void remove_context_tip();

    void set_context_menu(ContextMenu* p_context_menu, const vec2& pos);
    void remove_context_menu();

    virtual void handle_event(Event* e);

    void init(Viewport* _parent);
    void resize();
    virtual void draw();
    void update();

    void free();

    Control* get_focused() const;
    void focus(Control* ctrl);

    Control* raycast(const vec2& pos);
    Control* raycast_layer(const CanvasLayer& l, const vec2& pos);

    static void bind_methods();

    Array<CanvasLayer> layers;

   protected:
    Viewport* parent;

    ContextMenu* context_menu;
    ContextTip* context_tip;
    Dialog* dialog;

    Control* last_hover = NULL;
    Control* focused = NULL;

    Control* last_clicked;
    int click_time;
    int double_click_treshold;

    Vector<Control> scheduled_updates;

    vec2 size;

    Control* tip_shower;
    int tip_time;
    int wait_time;
    bool awaiting_tip;
};

#define DEFAULT_THEME CanvasData::get_singleton()->get_default_theme()

class CanvasTheme {
   public:
    CanvasTheme();

    Font* get_font() const;
    Texture2D* get_frame() const;
    Texture2D* get_highlight() const;

    Color get_highlight_color() const;
    Color get_selection_color() const;

   private:
    Font* font;
    Texture2D* frame;
    Texture2D* highlight;

    Color highlight_color;
    Color selection_color;
};

class CanvasData {
   public:
    CanvasData();

    static void init();

    static CanvasData* get_singleton();

    Shader* get_default_shader() const;
    CanvasTheme* get_default_theme() const;

   private:
    static CanvasData* singleton;

    Shader* default_shader;

    CanvasTheme* default_theme;
};
