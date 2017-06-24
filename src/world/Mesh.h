#pragma once

#include <string>

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "SDL.h"

#include "resources/Shader.h"
#include "resources/Texture.h"

struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class Material;

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

	void SetAttributes(Shader *shader);
	void bind();
	void unbind();
	void draw();

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

class Mesh
{
public:
	Mesh();
	Mesh(const String& p_path);

	~Mesh();
	
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
	};

	bool import(const String& p_filepath);

	void draw();

private:
	MeshNode* root;

	Vector<Texture2D> textures;
	Vector<MeshNode> meshes;
	Vector<Material> materials;
};

class Material
{
public:
	void load_material(const aiMaterial* p_material);
	Texture2D* load_texture(const aiMaterial* p_material, const aiTextureType &p_type);

	String name;

	Shader* shader;

	Texture2D* diffuse_texture;
	Texture2D* specular_texture;
	Texture2D* ambient_texture;

	Color diffuse_color;
	Color specular_color;
	Color ambient_color;
	Color emissive_color;

	float shininess;
	float opacity;
};

class MeshHandler
{
public:
	static MeshHandler* get_singleton();

	void Init();

	SimpleMesh* get_plane() const;
	SimpleMesh* get_line() const;

private:
	static MeshHandler *singleton;

	SimpleMesh *plane, *line;
};