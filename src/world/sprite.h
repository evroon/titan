#pragma once

#include "mesh.h"
#include "worldobject.h"

class Sprite : public WorldObject {
    OBJ_DEFINITION(Sprite, WorldObject)

   public:
    Sprite();
    Sprite(Texture2D *tex, Shader *shader);
    Sprite(Texture2D *tex, vec2 ts, Shader *shader);
    Sprite(Texture2D *tex, vec2 ts);
    Sprite(Texture2D *tex);
    Sprite(const String &tex_name);
    virtual ~Sprite();

    static void Init();

    // Set tile sizes
    void set_tilesize(const vec2 &size);

    // Get a tile's position and size at given index
    vec2 GetTilePosition(int index) const;
    vec4 GetTileBounds(int index) const;

    void set_shader(Shader *p_shader);
    Shader *get_shader() const;

    void set_texture(Texture2D *p_texture);
    Texture2D *get_texture() const;

    void set_bounds(const vec4 &p_bounds);
    vec4 get_bounds() const;

    // Flip bounds
    void flip_vertically();
    void flip_horizontally();

    // Draw
    void DrawMap();  // Draw in a 3x3 matrix structure
    void draw() override;
    void SimpleDraw() override;
    void update() override;

    static void bind_methods();

   private:
    static SimpleMesh *defaultmesh;

    // SpriteBatch info
    bool packed;
    int activeindex = 0;

    Shader *shader;
    Texture2D *texture;
    vec4 bounds;
    vec2 tilesize, tilecount;
};
