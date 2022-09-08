#pragma once

#include "mesh.h"
#include "worldobject.h"

class Terrain;

class TerrainNoise : public WorldObject {
    OBJ_DEFINITION(TerrainNoise, WorldObject);

   public:
    TerrainNoise();

    SIMPLE_GET_SET(float, octaves);

    static void bind_methods();

   private:
    Terrain* terrain;

    float octaves;
};
