#pragma once

#include <string>

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#else
#include "SDL.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#endif

#include "resources/Shader.h"
#include "resources/Texture.h"

class Material;
class Model;

struct BoundingBox
{
	vec3 min;
	vec3 max;
};

class SimpleMesh
{
public:
	struct Vertex
	{
		float position[3];
	};
	struct Face
	{
		GLubyte index[3];
	};

	~SimpleMesh();

	void init(const Array<Vertex>& p_vertices, const Array<Face>& p_faces, int p_type = GL_TRIANGLES);

	void set_plane();
	void set_line();
	void set_cube();

	void SetAttributes(Shader *shader);
	void bind();
	void unbind();

	void draw();
	void draw_instanced(int p_count);

private:
	Vertex* vertices;
	Face* faces;

	int faces_count;
	int vertices_count;

	GLuint position_id;
	GLuint VAO, VBO, EBO;
	GLuint novertices = 3;
	GLenum draw_type;
};

class Mesh : public Resource
{
	OBJ_DEFINITION(Mesh, Resource);

public:
	Mesh();
	Mesh(const String& p_path);

	~Mesh();

	friend class Model;
	
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texcoords;
	};

	struct VertexWeight
	{
		int vertex_index;
		float weight;
	};

	struct Bone
	{
		String name;
		Array<VertexWeight> weights;

		mat4 matrix;
	};

	struct Face
	{
		unsigned indices[3];
	};

	struct MeshNode
	{
		void draw();

		void init(aiMesh* p_mesh);
		void setup_buffers();

		String name;

		Array<Face> faces;
		Array<Vertex> vertices;
		Array<Color> colors;
		
		unsigned mat_index;
		Material* material;

		Mesh* parent;

		GLuint VAO, VBO, EBO;

		Color modulate;
	};

	bool import(const String& p_filepath);

	void draw();

	BoundingBox get_bounding_box() const;

	Array<Variant> get_materials() const;
	void set_materials(const Array<Variant>& p_materials);

	static void bind_methods();

private:
	BoundingBox get_bounding_box(Mesh* p_mesh, const mat4& p_transform);

	MeshNode* root;

	Vector<Texture2D> textures;
	Vector<MeshNode> meshes;
	Vector<Material> materials;

	BoundingBox bounding_box;

	Model* model;
};

class Material : public Resource
{
	OBJ_DEFINITION(Material, Resource);

	friend class Mesh;

public:
	void set_shader(Shader* p_shader);
	Shader* get_shader() const;

	String get_name() const;

	void set_diffuse_color(const Color& p_color);
	Color get_diffuse_color() const;

	void set_specular_color(const Color& p_color);
	Color get_specular_color() const;

	void set_ambient_color(const Color& p_color);
	Color get_ambient_color() const;

	void set_emissive_color(const Color& p_color);
	Color get_emissive_color() const;

	void set_shininess(float p_shininess);
	float get_shininess() const;
	
	Texture2D* get_diffuse_texture() const;
	Texture2D* get_specular_texture() const;
	Texture2D* get_ambient_texture() const;

	Mesh* get_mesh() const;

	static void bind_methods();

private:
	void load_material(const aiMaterial* p_material);
	Texture2D* load_texture(const aiMaterial* p_material, const aiTextureType &p_type);

	Vector<Texture2D> textures;

	String name;

	Shader* shader;
	Mesh* mesh;

	Texture2D* diffuse_texture;
	Texture2D* specular_texture;
	Texture2D* ambient_texture;

	Color diffuse_color;
	Color specular_color;
	Color ambient_color;
	Color emissive_color;

	float shininess;
};

class MeshHandler
{
public:
	static MeshHandler* get_singleton();

	void Init();

	SimpleMesh* get_plane() const;
	SimpleMesh* get_line() const;
	SimpleMesh* get_cube() const;

private:
	static MeshHandler *singleton;

	SimpleMesh* plane;
	SimpleMesh* line;
	SimpleMesh* cube;
};