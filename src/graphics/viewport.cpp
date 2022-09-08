#include "viewport.h"

#include "core/windowmanager.h"
#include "game/scene.h"
#include "renderer.h"
#include "ui/worldview.h"
#include "view.h"

Viewport::Viewport() {
    VIEW->bind_viewport(this);

    fbo = NULL;
    postprocess = NULL;
    parentview = NULL;
    camera = NULL;
    scene = NULL;
    world = NULL;
    canvas = NULL;
    renderer = NULL;
    return_viewport = NULL;
    wireframe_enabled = false;
    fps_locked = false;

    destination = DrawDestination::DIRECT;
    mode = DrawMode::FORWARD_RENDERING;

    graphics_transform = mat4();
    input_transform = mat4();

    limiter = RenderTarget();
    updatelim = FPSLimiter();

    delta_time = 0.0f;
    fps = 0.0f;
    fps_guess = 0.0f;

    renderarea = rect2();
}

Viewport::Viewport(Renderer* p_renderer) : Viewport() {
    set_renderer(p_renderer);
}

Viewport::~Viewport() { delete canvas; }

void Viewport::set_world(World* p_world) {
    if (world) remove_child(world);

    world = p_world;
    add_child(world);
}

World* Viewport::get_world() const { return world; }

void Viewport::set_canvas(Canvas* p_canvas) {
    if (p_canvas == NULL) {
        T_ERROR("Canvas is null...");
        return;
    }

    if (canvas) remove_child(canvas);

    canvas = p_canvas;
    add_child(canvas);
    canvas->resize();
}

Canvas* Viewport::get_canvas() const { return canvas; }

void Viewport::set_mode(DrawDestination p_mode) { destination = p_mode; }

int Viewport::get_mode() const { return destination; }

void Viewport::set_wireframe_enabled(bool p_wireframe_enabled) {
    wireframe_enabled = p_wireframe_enabled;
}

bool Viewport::get_wireframe_enabled() const { return wireframe_enabled; }

void Viewport::set_fbo(FBO2D* p_fbo) { fbo = p_fbo; }

FBO2D* Viewport::get_fbo() const { return fbo; }

void Viewport::set_postprocess(PostProcess* p_postprocess) {
    postprocess = p_postprocess;
    destination = POSTPROCESS;
}

PostProcess* Viewport::get_postprocess() const { return postprocess; }

float Viewport::get_fps() const { return fps; }

float Viewport::get_rendering_time() const {
    return rendering_stopwatch.result_averaged;
}

float Viewport::get_updating_time() const {
    return updating_stopwatch.result_averaged;
}

void Viewport::init() {
    if (canvas) canvas->init(this);

    if (world) world->init();
}

void Viewport::bind_parent(View* p_parentview) { parentview = p_parentview; }

vec2 Viewport::get_size() const { return renderarea.size; }

void Viewport::resize(const rect2& p_area) {
    renderarea = p_area;

    vec2 size = p_area.size;
    vec2 window_size = WINDOWSIZE_F;

    mat4 translate, rotate, scale;

    translate.translate(
        vec3(renderarea.pos.x / size.x, renderarea.pos.y / size.y, 0));
    scale.scale(vec3(1.0f / size.x, 1.0f / size.y, 0.0f));

    graphics_transform = scale;

    vec3 s = vec3(size * 2.0f / window_size, 0.0f);
    // input_transform = mat4::Scale(vec3(size / window_size, 0));
    input_transform = mat4::Translate(vec3(-p_area.pos.x, -p_area.pos.y, 0));

    if (canvas) {
        canvas->resize();
    }

    if (renderer) renderer->resized();
}

rect2 Viewport::get_area() const { return renderarea; }

vec2 Viewport::get_screen_coords(const vec2& p_pos) const {
    if (!world) return vec2();

    vec4 p = {(p_pos - renderarea.pos), 0.0f, 1.0f};

    Camera* cam = world->get_active_camera();
    mat4 inv = cam->get_inverse();

    return p.get_xy();
}

bool Viewport::is_overlapping(const rect2& area) const {
    return renderarea.is_overlapping(area);
}

bool Viewport::is_in_box(const vec2& pos) const {
    return renderarea.is_in_box(pos);
}

bool Viewport::is_in_box(const rect2& area) const {
    return renderarea.is_in_box(area);
}

void Viewport::set_renderer(Renderer* p_renderer) {
    renderer = p_renderer;
    renderer->set_viewport(this);
    renderer->activate();
}

Renderer* Viewport::get_renderer() const { return renderer; }

void Viewport::set_scene(Scene* p_scene) {
    scene = p_scene;
    set_world(scene->get_child_by_type<World*>());
    set_canvas(scene->get_child_by_type<Canvas*>());
}

Scene* Viewport::get_scene() const { return scene; }

void Viewport::activate() {
    return_viewport = VIEW->get_active_viewport();
    VIEW->set_active_viewport(this);
    renderer->activate();
}

void Viewport::deactivate() {
    if (!return_viewport) return;

    return_viewport->activate();
    return_viewport = NULL;
}

mat4 Viewport::get_input_transform() const { return input_transform; }

void Viewport::post_draw_world() {}

void Viewport::post_draw_canvas() {}

void Viewport::draw() {
    bool s = limiter.should_update();
    fps = to_float(limiter.fps_measured);
    fps_guess = to_float(limiter.fps_guessed);

    if (s && renderer) {
        rendering_stopwatch.start();
        activate();
        renderer->render();
        deactivate();
        rendering_stopwatch.stop();
    }
}

void Viewport::update() {
    // Handle Update Loop
    FPSLimiter::FPSInfo i;
    i = updatelim.update();
    delta_time = i.delta_time;

    if (i.needsupdate) {
        updating_stopwatch.start();
        activate();

        if (world) world->update();

        if (canvas) canvas->update();

        deactivate();
        updating_stopwatch.stop();
    }
}

void Viewport::handle_event(Event* event) {
    // activate_transform();

    if (world) world->handle_event(event);

    if (canvas) canvas->handle_event(event);

    // deactivate_transform();
}

vec3 Viewport::deferred_raycast(const vec2& p_pos) const {
    return DEFERRED_RENDERER->get_position_at_pixel(p_pos);
}

Object* Viewport::raycast(const vec2& pos) const {
    vec2 projected = get_screen_coords(pos);

    if (world) {
        for (int c = 0; c < world->get_child_count(); c++) {
            WorldObject* obj =
                world->get_child_by_index(c)->cast_to_type<WorldObject*>();

            if (obj->is_of_type<Camera>()) continue;

            if (obj->CheckOverlap(projected))
                return world->get_child_by_index(c);
        }
    }

    return nullptr;
}

#undef CLASSNAME
#define CLASSNAME Viewport

void Viewport::bind_methods() {
    REG_PROPERTY(world);
    REG_PROPERTY(canvas);
    REG_PROPERTY(wireframe_enabled);
}

EditorViewport::EditorViewport(Renderer* p_renderer) : Viewport(p_renderer) {}

void EditorViewport::post_draw_world() { worldview->post_draw_world(); }

void EditorViewport::post_draw_canvas() { worldview->post_draw_canvas(); }
