#include "view.h"

#include "core/application.h"
#include "core/titanscript/titanscript.h"
#include "world/sprite.h"

View View::singleton;

View::View() {
    viewports = Vector<Viewport>();

    default_viewport = nullptr;
    active_viewport = nullptr;
    application = nullptr;

    fps_locked = false;
}

View::~View() {}

void View::init(const vec2& p_default_size) {
    default_viewport->resize(p_default_size);
}

void View::draw() { default_viewport->draw(); }

void View::update() { default_viewport->update(); }

void View::handle_event(Event* event) { default_viewport->handle_event(event); }

void View::bind_viewport(Viewport* viewport) {
    viewport->bind_parent(this);
    viewports.push_back(viewport);
}

void View::free() {}

void View::set_default_viewport(Viewport* p_viewport) {
    default_viewport = p_viewport;
}

Viewport* View::get_default_viewport() const { return default_viewport; }

void View::set_active_viewport(Viewport* p_viewport) {
    active_viewport = p_viewport;
}

Viewport* View::get_active_viewport() const { return active_viewport; }

void View::set_application(Application* p_application) {
    application = p_application;
}

Application* View::get_application() const { return application; }

View* View::get_singleton() { return &singleton; }

#undef CLASSNAME
#define CLASSNAME View

void View::bind_methods() { REG_PROPERTY(default_viewport); }
