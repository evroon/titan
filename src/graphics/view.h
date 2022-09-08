#pragma once

#include "core/object.h"
#include "viewport.h"

#define VIEW View::get_singleton()
#define ACTIVE_VIEWPORT VIEW->get_active_viewport()
#define ACTIVE_CANVAS ACTIVE_VIEWPORT->get_canvas()
#define ACTIVE_WORLD ACTIVE_VIEWPORT->get_world()
#define EDITOR_APP (reinterpret_cast<EditorApp*>(VIEW->get_application()))

class Application;

class View : public Object {
    OBJ_DEFINITION(View, Object);

   public:
    View();
    virtual ~View();

    void init(const vec2& p_default_size);

    void draw();
    void update();
    void bind_viewport(Viewport* viewport);
    void handle_event(Event* event);
    void free();

    void set_default_viewport(Viewport* p_viewport);
    Viewport* get_default_viewport() const;

    void set_active_viewport(Viewport* p_viewport);
    Viewport* get_active_viewport() const;

    void set_application(Application* p_application);
    Application* get_application() const;

    static View* get_singleton();

    static void bind_methods();

   private:
    static View singleton;

    Vector<Viewport> viewports;

    Viewport* default_viewport;
    Viewport* active_viewport;

    Application* application;

    bool fps_locked;
};
