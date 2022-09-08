#pragma once

#include "worldobject.h"

class Environment : public WorldObject {
    OBJ_DEFINITION(Environment, WorldObject);

   public:
    Environment();
    virtual ~Environment();

    void set_ambient_color(const Color &p_ambient_color);
    Color get_ambient_color() const;

    void set_auto_exposure_enabled(bool p_auto_exposure_enabled);
    bool get_auto_exposure_enabled() const;

    void set_fog_enabled(bool p_fog_enabled);
    bool get_fog_enabled() const;

    void set_fog_density(float p_fog_density);
    float get_fog_density() const;

    void set_fog_gradient(float p_fog_gradient);
    float get_fog_gradient() const;

    SIMPLE_GET_SET(bool, dof_enabled);
    SIMPLE_GET_SET(float, dof_rate);
    SIMPLE_GET_SET(float, dof_focus);

    SIMPLE_GET_SET(bool, ssao_enabled);
    SIMPLE_GET_SET(float, ssao_radius);

    SIMPLE_GET_SET(bool, bloom_enabled);
    SIMPLE_GET_SET(float, bloom_threshold);

    SIMPLE_GET_SET(float, exposure);
    SIMPLE_GET_SET(float, gamma);

    static void bind_methods();

   private:
    // Ambient
    Color ambient_color;

    // Auto exposure
    bool auto_exposure_enabled;
    float auto_exposure_speed;
    float auto_exposure_max;
    float auto_exposure_min;
    float exposure;
    float gamma;

    // Bloom
    bool bloom_enabled;
    float bloom_threshold;

    // Fog
    bool fog_enabled;
    float fog_density;
    float fog_gradient;
    Color fog_color;

    // DoF
    bool dof_enabled;
    float dof_rate;
    float dof_focus;

    // Godrays
    bool godrays_enabled;

    // Lighting
    bool lighting_enabled;
    bool volumetric_lighting_enabled;

    // Tonemapping
    int tonemap_type;

    // SSAO
    bool ssao_enabled;
    float ssao_radius;
};
