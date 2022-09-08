#pragma once

#include "core/definitions.h"
#include "core/object.h"

#if PLATFORM == WINDOWS
#include "module/perlin.h"
#include "noise.h"
#else
#include "libnoise/module/perlin.h"
#include "libnoise/noise.h"
#endif

class Texture2D;
class Texture3D;

class Noise : public Object {
    OBJ_DEFINITION(Noise, Object);

   public:
    Noise();
    virtual ~Noise();
};

class PerlinNoise : public Noise {
    OBJ_DEFINITION(PerlinNoise, Noise);

   public:
    PerlinNoise();

    float get_value(const vec3 &p_pos);

    Texture2D *create_2d_texture(const vec2i &p_size);
    Texture3D *create_3d_texture();

   private:
    static void fill_partition(PerlinNoise *p_noise, vec4 *p_heights, int p_x,
                               int p_y, const vec2i &p_partition_size,
                               const vec2i &p_size);
    noise::module::Perlin perlin_noise;
};

class WorleyNoise : public Noise {
    OBJ_DEFINITION(WorleyNoise, Noise);

   public:
    WorleyNoise() : WorleyNoise(16.0f) {}
    WorleyNoise(const vec3 &p_size);

    float get_value(const vec3 &p_pos);

   private:
    Array<vec3> spheres;
    vec3 size;
};
