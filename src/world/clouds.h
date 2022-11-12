#pragma once

#include "worldobject.h"

class Clouds : public WorldObject {
    OBJ_DEFINITION(Clouds, WorldObject);

   public:
    Clouds();

    static void bind_methods();

   private:
    Texture3D* texture;
    Shader* shader;
};
