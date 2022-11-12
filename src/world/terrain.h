#pragma once

#include "mesh.h"
#include "worldobject.h"

class Terrain;
class FBO2D;

class TerrainBrush : public Object {
    OBJ_DEFINITION(TerrainBrush, Object);

   public:
    TerrainBrush(Terrain* p_terrain);

    friend class Terrain;

    void apply();
    void handle();

    void set_radius(float p_radius);
    float get_radius() const;

    void set_deviation(float p_deviation);
    float get_deviation() const;

    void set_color(const Color& p_color);
    Color get_color() const;

    void set_strength(float p_strength);
    float get_strength() const;

    void set_pos(const vec2& p_pos);
    vec2 get_pos() const;

    FBO2D* get_fbo() const;

    static void bind_methods();

   private:
    Terrain* terrain;

    FBO2D* heightmap_fbo;
    Shader* brush_shader;
    Array<Ref<Texture2D>> textures;

    int active_tex;

    float radius;
    float deviation;
    float strength;

    Color color;
    vec2 pos;

    bool to_apply;
};

class Terrain : public WorldObject {
    OBJ_DEFINITION(Terrain, WorldObject);

   public:
    Terrain();
    virtual ~Terrain();

    struct TerrainNode {
        vec2 position;
    };

    friend class TerrainBrush;

    void init();
    void build();
    void compute_normals();

    void increase(float p_amount);
    void smooth(float p_amount);

    void update_buffer();
    void update_buffer_range(const vec2& p_start, const vec2 p_end);
    void setup_buffers();

    float get_height(const vec2& p_pos) const;
    float get_height_fast(const vec2& p_pos) const;
    Ref<Texture2D> get_heightmap() const;

    TerrainBrush* get_brush() const;

    void set_selection_pos(const vec2& p_pos);

    void ready() override;
    void draw() override;

    void set_tex_name(int p_index, const String& p_tex_name);

    static void bind_methods();

   private:
    vec2i node_count;

    GLuint VAO, VBO, EBO;

    Array<TerrainNode> nodes;

    Shader* shader;
    Shader* normal_map_compute;

    TerrainBrush* brush;

    Ref<Texture2D> normalmap;
    Ref<Texture2D> heightmap;

    Vector<Texture2D> textures;
    Array<String> texture_names;

    Color diffuse_color;

    float height_factor;
};

class Water : public WorldObject {
    OBJ_DEFINITION(Water, WorldObject);

   public:
    Water();

    void draw() override;

    static void bind_methods();

   private:
    Shader* shader;

    Texture2D* normals;
};

class Model;

class Vegetation : public WorldObject {
    OBJ_DEFINITION(Vegetation, WorldObject);

   public:
    Vegetation();
    Vegetation(Terrain* p_parent);

    void init();

    void draw() override;

    void update_buffer();

    void set_terrain(Terrain* p_terrain);
    Terrain* get_terrain() const;

    static void bind_methods();

   private:
    Shader* shader;
    Texture2D* grass_tex;
    Terrain* terrain;

    Vector<Model> models;

    Array<mat4> model_matrices;
    Array<float> positions;
};
