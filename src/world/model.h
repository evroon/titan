#pragma once

#include "mesh.h"
#include "worldobject.h"

class Model : public WorldObject {
    OBJ_DEFINITION(Model, WorldObject);

   public:
    Model();
    Model(const String& p_path);
    Model(Mesh* p_mesh);

    virtual ~Model();

    friend class Mesh;

    void draw() override;
    void shadow_draw() override;

    void load_mesh(const String& p_path);

    void set_mesh(Mesh* p_mesh);
    Mesh* get_mesh() const;

    void set_color_id(const vec3& p_color_id);
    vec3 get_color_id() const;

    BoundingBox get_bounding_box() const;

    static void bind_methods();

   private:
    Mesh* mesh;
    Shader* shader;

    vec3 color_id;
};
