#include "mesh.h"

#include "core/contentmanager.h"
#include "graphics/renderer.h"
#include "model.h"
#include "resources/file.h"

MeshHandler* MeshHandler::singleton = new MeshHandler;

//=========================================================================
// SimpleMesh
//=========================================================================

void SimpleMesh::init(const Array<Vertex>& p_vertices, const Array<Face>& p_faces, int p_type) {
    draw_type = p_type;
    vertices_count = p_vertices.size();
    faces_count = p_faces.size();

    vertices = new Vertex[vertices_count];
    faces = new Face[faces_count];

    for (int c = 0; c < vertices_count; c++) vertices[c] = p_vertices[c];

    for (int c = 0; c < p_faces.size(); c++) faces[c] = p_faces[c];

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * faces_count, &faces[0], GL_STATIC_DRAW);
}

SimpleMesh::~SimpleMesh() {
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

void SimpleMesh::set_plane() {
    const Array<Vertex> vertices = {{1, -1, 0}, {1, 1, 0}, {-1, 1, 0}, {-1, -1, 0}};

    const Array<Face> faces = {{0, 1, 2}, {2, 3, 0}};

    init(vertices, faces, GL_TRIANGLES);
}

void SimpleMesh::set_line() {
    const Array<Vertex> vertices = {{0, 0, 0}, {1, 0, 0}};

    const Array<Face> faces = {{0, 1}};

    init(vertices, faces, GL_LINES);
}

void SimpleMesh::set_cube() {
    Array<Vertex> vertices;
    Array<Face> faces;

    vertices.reserve(8);
    faces.reserve(12);

    vertices.push_back({-1.0, -1.0, 1.0});
    vertices.push_back({1.0, -1.0, 1.0});
    vertices.push_back({1.0, 1.0, 1.0});
    vertices.push_back({-1.0, 1.0, 1.0});

    vertices.push_back({-1.0, -1.0, -1.0});
    vertices.push_back({1.0, -1.0, -1.0});
    vertices.push_back({1.0, 1.0, -1.0});
    vertices.push_back({-1.0, 1.0, -1.0});

    faces.push_back({1, 5, 6});
    faces.push_back({6, 2, 1});

    faces.push_back({0, 1, 2});
    faces.push_back({2, 3, 0});

    faces.push_back({7, 6, 5});
    faces.push_back({5, 4, 7});

    faces.push_back({4, 0, 3});
    faces.push_back({3, 7, 4});

    faces.push_back({4, 5, 1});
    faces.push_back({1, 0, 4});

    faces.push_back({3, 2, 6});
    faces.push_back({6, 7, 3});

    init(vertices, faces, GL_TRIANGLES);
}

void SimpleMesh::SetAttributes(Shader* shader) {
    position_id = glGetAttribLocation(shader->get_program(), "a_position");

    if (position_id < 0) T_ERROR("attribute location invalid");

    glEnableVertexAttribArray(position_id);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void SimpleMesh::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void SimpleMesh::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleMesh::draw() {
    glEnableVertexAttribArray(0);

    glDrawElements(draw_type, faces_count * (draw_type == GL_TRIANGLES ? 3 : 2), GL_UNSIGNED_BYTE,
                   0);

    glDisableVertexAttribArray(0);
}

void SimpleMesh::draw_instanced(int p_count) {
    glEnableVertexAttribArray(0);

    glDrawElementsInstanced(draw_type, faces_count * (draw_type == GL_TRIANGLES ? 3 : 2),
                            GL_UNSIGNED_BYTE, 0, p_count);

    glDisableVertexAttribArray(0);
}

//=========================================================================
// Mesh
//=========================================================================

Mesh::Mesh() {}

Mesh::Mesh(const String& p_path) {
    file = p_path;
    import(p_path);
}

Mesh::~Mesh() {
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

BoundingBox Mesh::get_bounding_box(Mesh* p_mesh, const mat4& p_transform) {
    BoundingBox box;

    for (int c = 0; c < p_mesh->meshes.size(); c++) {
        MeshNode* node = p_mesh->meshes[c];

        for (int t = 0; t < node->vertices.size(); ++t) {
            vec3 tmp = node->vertices[t].position;

            box.min.x = MIN(box.min.x, tmp.x);
            box.min.y = MIN(box.min.y, tmp.y);
            box.min.z = MIN(box.min.z, tmp.z);

            box.max.x = MAX(box.max.x, tmp.x);
            box.max.y = MAX(box.max.y, tmp.y);
            box.max.z = MAX(box.max.z, tmp.z);
        }
    }

    return box;
}

bool Mesh::import(const String& p_filepath) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        p_filepath, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                        aiProcess_SortByPType | aiProcess_GenSmoothNormals);

    if (!scene) {
        T_ERROR(importer.GetErrorString());
        return false;
    }

    for (unsigned c = 0; c < scene->mNumMeshes; c++) {
        MeshNode* node = new MeshNode;
        node->init(scene->mMeshes[c]);
        node->parent = this;
        meshes.push_back(node);
    }

    for (unsigned c = 0; c < scene->mNumTextures; c++)
        textures.push_back(new Texture2D(scene->mTextures[c]));

    for (unsigned c = 0; c < scene->mNumMaterials; c++) {
        Material* material = new Material;
        material->mesh = this;
        material->load_material(scene->mMaterials[c]);
        materials.push_back(material);
    }

    for (int c = 0; c < meshes.size(); c++) {
        unsigned mat_index = meshes[c]->mat_index;
        meshes[c]->material = materials[mat_index];

        bounding_box = get_bounding_box(this, mat4());
    }

    return true;
}

void Mesh::draw() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (MeshNode* mesh : meshes) mesh->draw();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

BoundingBox Mesh::get_bounding_box() const { return bounding_box; }

Array<Variant> Mesh::get_materials() const {
    Array<Variant> v;

    for (int c = 0; c < materials.size(); c++) v.push_back(materials[c]);

    return v;
}

void Mesh::set_materials(const Array<Variant>& p_materials) {}

#undef CLASSNAME
#define CLASSNAME Mesh

void Mesh::bind_methods() {
    REG_CSTR(0);
    REG_CSTR_OVRLD_1(String);

    REG_PROPERTY(materials);
}

//=========================================================================
// MeshNode
//=========================================================================

void Mesh::MeshNode::draw() {
    glBindVertexArray(VAO);

    if (material) {
        if (material->get_diffuse_texture()) {
            RENDERER->use_blending();
            material->get_shader()->set_uniform("texture_enabled", true);
            material->get_diffuse_texture()->bind(0);
        } else {
            material->get_shader()->set_uniform("texture_enabled", false);
        }
    }

    RENDERER->use_blending();

    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Mesh::MeshNode::init(aiMesh* p_mesh) {
    mat_index = p_mesh->mMaterialIndex;

    // add vertices
    for (GLuint i = 0; i < p_mesh->mNumVertices; i++) {
        Vertex vertex;

        aiVector3D v = p_mesh->mVertices[i];
        aiVector3D n = p_mesh->mNormals[i];
        aiVector3D t;

        if (p_mesh->mTextureCoords[0]) t = p_mesh->mTextureCoords[0][i];

        vertex.position = vec3(v.x, v.y, v.z);
        vertex.normal = vec3(n.x, n.y, n.z);
        vertex.texcoords = vec2(t.x, t.y - 1.0);

        vertices.push_back(vertex);
    }

    // add faces
    for (unsigned i = 0; i < p_mesh->mNumFaces; i++) {
        Face face;

        for (int c = 0; c < 3; c++) face.indices[c] = p_mesh->mFaces[i].mIndices[c];

        faces.push_back(face);
    }

    setup_buffers();
}

void Mesh::MeshNode::setup_buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), &faces[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));

    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texcoords));

    glBindVertexArray(0);
}

//=========================================================================
// Material
//=========================================================================

Material::Material() {
    shader = nullptr;
    mesh = nullptr;
    diffuse_texture = nullptr;
    specular_texture = nullptr;
    ambient_texture = nullptr;

    diffuse_color = Color();
    specular_color = Color();
    ambient_color = Color();
    emissive_color = Color();

    name = "";

    textures = Vector<Texture2D>();
    shininess = 0.0f;
}

void Material::load_material(const aiMaterial* p_material) {
    // initialize values
    aiString name;
    aiColor3D color;
    float value;

    p_material->Get(AI_MATKEY_NAME, name);
    name = name.C_Str();

    p_material->Get(AI_MATKEY_OPACITY, value);
    float opacity = value;

    p_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    diffuse_color = Color(color.r, color.g, color.b, opacity);

    p_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    specular_color = Color(color.r, color.g, color.b);

    p_material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    ambient_color = Color(color.r, color.g, color.b);

    p_material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    emissive_color = Color(color.r, color.g, color.b);

    p_material->Get(AI_MATKEY_SHININESS, value);
    shininess = value;

    // initialize textures
    diffuse_texture = load_texture(p_material, aiTextureType_DIFFUSE);
    specular_texture = load_texture(p_material, aiTextureType_SPECULAR);
    ambient_texture = load_texture(p_material, aiTextureType_AMBIENT);

    shader = CONTENT->LoadShader("engine/shaders/Shader3D");
}

Texture2D* Material::load_texture(const aiMaterial* p_material, const aiTextureType& p_type) {
    if (p_material->GetTextureCount(p_type) > 0) {
        aiString Path;

        if (p_material->GetTexture(p_type, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            File f = mesh->get_file();
            f.go_up();
            String p = String(Path.C_Str());
            p.remove('/');
            String path = f.operator String() + "/" + p;

            return CONTENT->LoadTexture(path);
        }
    }
    return nullptr;
}

#undef CLASSNAME
#define CLASSNAME Material

void Material::set_shader(Shader* p_shader) { shader = p_shader; }

Shader* Material::get_shader() const { return shader; }

String Material::get_name() const { return name; }

void Material::set_diffuse_color(const Color& p_color) {}

Color Material::get_diffuse_color() const { return diffuse_color; }

void Material::set_specular_color(const Color& p_color) {}

Color Material::get_specular_color() const { return specular_color; }

void Material::set_ambient_color(const Color& p_color) { ambient_color = p_color; }

Color Material::get_ambient_color() const { return ambient_color; }

void Material::set_emissive_color(const Color& p_color) { emissive_color = p_color; }

Color Material::get_emissive_color() const { return emissive_color; }

void Material::set_shininess(float p_shininess) {}

float Material::get_shininess() const { return shininess; }

Texture2D* Material::get_diffuse_texture() const { return diffuse_texture; }

Texture2D* Material::get_specular_texture() const { return specular_texture; }

Texture2D* Material::get_ambient_texture() const { return ambient_texture; }

Mesh* Material::get_mesh() const { return mesh; }

#undef CLASSNAME
#define CLASSNAME Material

void Material::bind_methods() {
    REG_PROPERTY(shininess);
    REG_PROPERTY(emissive_color);
    REG_PROPERTY(ambient_color);
    REG_PROPERTY(diffuse_color);
    REG_PROPERTY(shader);
}

//=========================================================================
// MeshHandler
//=========================================================================

void MeshHandler::Init() {
    plane = new SimpleMesh;
    plane->set_plane();

    line = new SimpleMesh;
    line->set_line();

    cube = new SimpleMesh;
    cube->set_cube();
}

SimpleMesh* MeshHandler::get_plane() const { return plane; }

SimpleMesh* MeshHandler::get_line() const { return line; }

SimpleMesh* MeshHandler::get_cube() const { return cube; }

MeshHandler* MeshHandler::get_singleton() { return singleton; }
