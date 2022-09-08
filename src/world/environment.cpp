#include "environment.h"

Environment::Environment() {
    ambient_color = Color(0.5f, 0.5f, 0.5f, 1.0f);
    auto_exposure_enabled = true;
    godrays_enabled = false;
    fog_enabled = false;
    fog_density = 0.001f;
    fog_gradient = 1.0f;
    ssao_enabled = true;
}

Environment::~Environment() {}

void Environment::set_ambient_color(const Color& p_ambient_color) {
    ambient_color = p_ambient_color;
}

Color Environment::get_ambient_color() const { return ambient_color; }

void Environment::set_auto_exposure_enabled(bool p_auto_exposure_enabled) {
    auto_exposure_enabled = p_auto_exposure_enabled;
}

bool Environment::get_auto_exposure_enabled() const {
    return auto_exposure_enabled;
}

void Environment::set_fog_enabled(bool p_fog_enabled) {
    fog_enabled = p_fog_enabled;
}

bool Environment::get_fog_enabled() const { return fog_enabled; }

void Environment::set_fog_density(float p_fog_density) {
    fog_density = p_fog_density;
}

float Environment::get_fog_density() const { return fog_density; }

void Environment::set_fog_gradient(float p_fog_gradient) {
    fog_gradient = p_fog_gradient;
}

float Environment::get_fog_gradient() const { return fog_gradient; }

#undef CLASSNAME
#define CLASSNAME Environment

void Environment::bind_methods() {
    REG_CSTR(0);

    REG_PROPERTY(ambient_color);
    REG_PROPERTY(auto_exposure_enabled);
    REG_PROPERTY(exposure);
    REG_PROPERTY(gamma);
    REG_PROPERTY(fog_enabled);
    REG_PROPERTY(fog_density);
    REG_PROPERTY(fog_gradient);
    REG_PROPERTY(dof_enabled);
    REG_PROPERTY(dof_rate);
    REG_PROPERTY(dof_focus);
    REG_PROPERTY(bloom_threshold);
    REG_PROPERTY(ssao_enabled);
    REG_PROPERTY(ssao_radius);
    REG_PROPERTY(bloom_enabled);
    REG_PROPERTY(bloom_threshold);
}
