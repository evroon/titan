#include "clouds.h"

Clouds::Clouds() {
    PerlinNoise n;
    texture = n.create_3d_texture();
}

#undef CLASSNAME
#define CLASSNAME Clouds

void Clouds::bind_methods() { REG_CSTR(0); }
