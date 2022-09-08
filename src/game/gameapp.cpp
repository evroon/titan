#include "gameapp.h"

#include "core/project.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "input/input.h"

GameApp::GameApp(Platform *p_platform) : Application(p_platform) {
    activescene = nullptr;
}

void GameApp::init() {
    Renderer *r = new DeferredRenderer;
    Viewport *v = new Viewport(r);
    VIEW->set_default_viewport(v);
    VIEW->set_active_viewport(v);

    Project *project = new Project("projects/terrain.xml");
    start_scene(project->get_main_scene());
}

void GameApp::update() {
    activescene->update();

    for (Event *e : INPUT->events) VIEW->handle_event(e);

    VIEW->update();
}

void GameApp::draw() { VIEW->draw(); }

void GameApp::resize(const vec2i &p_size) {
    VIEW->get_default_viewport()->resize(
        rect2(vec2(), vec2((float)p_size.x, (float)p_size.y) / 2.0f));
}

void GameApp::start_scene(Scene *s) {
    activescene = s;
    ACTIVE_VIEWPORT->set_scene(activescene);

    Camera *active_camera =
        ACTIVE_VIEWPORT->get_world()->get_child_by_type<Camera *>();

    if (!active_camera) {
        active_camera = new Camera;
        ACTIVE_VIEWPORT->get_world()->add_child(active_camera);
    }
    ACTIVE_VIEWPORT->get_world()->set_active_camera(active_camera);
}

void GameApp::handle_event(Event *e) { activescene->handle_event(e); }
