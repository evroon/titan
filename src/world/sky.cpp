#include "sky.h"

#include "core/contentmanager.h"
#include "core/time.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "mesh.h"

Sky::Sky() {
    shader = CONTENT->LoadShader("engine/shaders/Sky");

    set_size(vec3(10.0f, 10.0f, 10.0f));
    set_pos(vec3(0, 0, 0.0f));

    sky_color = Color::FromRGB(vec3i(66, 182, 244)).get_rgb();

    sun_direction = vec3(0, -1, 0);
}

Sky::~Sky() {}

void Sky::update() {
    float t = TIME->get_gametime() / 1.0e6f;
    float period = 600.0f;

    float theta = Math::modulo(t / period, 1.0f) * TWOPI;

    Color day_color = Color::FromRGB(vec3i(66, 182, 244)).get_rgb();
    Color dawn_color = Color::FromRGB(vec3i(255, 60, 60)).get_rgb();
    Color night_color = Color::FromRGB(vec3i(0)).get_rgb();
    Color color = day_color;

    float edge = PI / 8.0;
    float edge1 = -PI / 8.0f;

    if (theta + edge1 < edge) {
        float x = theta / edge;
        color = day_color * x + night_color * (1.0f - x);
    } else if (theta < PI - edge) {
        float x = PI - theta / edge;
        color = day_color * x + night_color * (1.0f - x);
    } else if (theta > TWOPI - edge) {
        float x = -(theta - TWOPI) / edge;
        color = night_color * x + night_color * (1.0f - x);
    } else if (theta > edge - PI) {
        float x = -theta / edge;
        color = night_color * x + night_color * (1.0f - x);
    }

    sky_color = day_color;
    sun_color = Color::FromRGB(vec3i(255, 120, 120));

    sun_direction.y = Math::cos(theta);
    sun_direction.z = Math::sin(theta);
}

#include "light.h"

void Sky::draw() {
    RENDERER->stop_culling();

    DirectionalLight* light = ACTIVE_WORLD->get_child_by_type<DirectionalLight*>();

    if (!light) return;

    vec3 light_dir = light->get_direction();

    shader->bind();
    shader->set_uniform("view", RENDERER->get_final_matrix());
    shader->set_uniform("sun_direction", light_dir);
    shader->set_uniform("sky_color", get_sky_color().get_rgb());
    shader->set_uniform("camera_position", ACTIVE_WORLD->get_active_camera()->get_pos());

    MeshHandler::get_singleton()->get_cube()->bind();
    MeshHandler::get_singleton()->get_cube()->draw();
    MeshHandler::get_singleton()->get_cube()->unbind();

    RENDERER->use_culling();
}

void Sky::set_sun_color(const Color& p_sun_color) { sun_color = p_sun_color; }

Color Sky::get_sun_color() const { return sun_color; }

void Sky::set_sky_color(const Color& p_sky_color) { sky_color = p_sky_color; }

Color Sky::get_sky_color() const { return sky_color; }

#undef CLASSNAME
#define CLASSNAME Sky

void Sky::bind_methods() {
    REG_CSTR(0);

    REG_PROPERTY(sky_color);
    REG_PROPERTY(sun_color);
}
